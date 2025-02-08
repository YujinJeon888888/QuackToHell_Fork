// Copyright_Team_AriAri

#include "GodFunction.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Json.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

// JSON 파일을 읽는 함수
FString UGodFunction::ReadFileContent(const FString& FilePath)
{
    FString Content;
    if (!FFileHelper::LoadFileToString(Content, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read file: %s"), *FilePath);
    }
    return Content;
}

// OpenAI API Key 로드 함수 추가 (가장 위쪽에 배치)
FString UGodFunction::LoadOpenAIKey()
{
    FString ConfigPath = FPaths::ProjectDir() + TEXT("OpenAIAuth.ini");
    FString FileContent;

    if (FFileHelper::LoadFileToString(FileContent, *ConfigPath))
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully loaded OpenAIAuth.ini"));
        FString ApiKey;
        if (FileContent.Split(TEXT("="), nullptr, &ApiKey))
        {
            return ApiKey.TrimStartAndEnd();
        }
    }

    UE_LOG(LogTemp, Error, TEXT("Failed to load OpenAI API Key from OpenAIAuth.ini"));
    return TEXT("");
}

// JSON Escape 함수 추가 (OpenAI API 관련 함수들 위쪽)
FString UGodFunction::EscapeJSON(const FString& Input)
{
    FString Escaped = Input;
    Escaped.ReplaceInline(TEXT("\\"), TEXT("\\\\"));
    Escaped.ReplaceInline(TEXT("\""), TEXT("\\\""));
    Escaped.ReplaceInline(TEXT("\n"), TEXT("\\n"));
    Escaped.ReplaceInline(TEXT("\r"), TEXT(""));
    Escaped.ReplaceInline(TEXT("\t"), TEXT("\\t"));

    return Escaped;
}


FString UGodFunction::ExtractValidJson(FString AIResponse)
{
    // OpenAI 응답을 JSON 객체로 파싱
    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(AIResponse);

    if (FJsonSerializer::Deserialize(Reader, RootObject) && RootObject->HasField("choices"))
    {
        TArray<TSharedPtr<FJsonValue>> Choices = RootObject->GetArrayField("choices");

        if (Choices.Num() > 0 && Choices[0]->AsObject()->HasField("message"))
        {
            FString JsonContent = Choices[0]->AsObject()->GetObjectField("message")->GetStringField("content");

            // Markdown 코드 블록(```json ... ```) 제거
            JsonContent.ReplaceInline(TEXT("```json"), TEXT(""));
            JsonContent.ReplaceInline(TEXT("```"), TEXT(""));
            JsonContent = JsonContent.TrimStartAndEnd();

            UE_LOG(LogTemp, Log, TEXT("Extracted JSON Content: %s"), *JsonContent);
            return JsonContent;
        }
    }

    UE_LOG(LogTemp, Error, TEXT("Failed to extract valid JSON from AI response!"));
    return TEXT("");
}

FString UGodFunction::CleanUpJson(FString JsonString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        if (JsonObject->HasField("name"))
        {
            FString Name = JsonObject->GetStringField("name");

            // JSON 내에 같은 이름이 여러 번 들어가면 제거
            JsonObject->SetStringField("name", Name);
        }

        FString CleanJson;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&CleanJson);
        FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

        UE_LOG(LogTemp, Log, TEXT("Cleaned JSON: %s"), *CleanJson);
        return CleanJson;
    }

    UE_LOG(LogTemp, Error, TEXT("Failed to clean JSON!"));
    return JsonString;
}

// JSON 파일 저장 함수
bool UGodFunction::SavePromptToFile(const FString& FileName, const FString& Content)
{
    FString FilePath = FPaths::ProjectSavedDir() + TEXT("Prompt/") + FileName;

    // 기존 파일 확인
    if (FPaths::FileExists(FilePath))
    {
        FString ExistingContent;
        FFileHelper::LoadFileToString(ExistingContent, *FilePath);

        if (!ExistingContent.IsEmpty() && !Content.IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("%s 파일이 이미 존재하며, 덮어쓰지 않음."), *FileName);
            return false;
        }
    }

    // 빈 JSON이면 저장하지 않음
    if (Content.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Empty content detected. Skipping file save for %s"), *FileName);
        return false;
    }

    bool bSuccess = FFileHelper::SaveStringToFile(Content, *FilePath);
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully saved prompt file: %s"), *FileName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save prompt file: %s"), *FileName);
    }

    return bSuccess;
}

// OpenAI API 호출 함수
void UGodFunction::CallOpenAIAsync(const FString& Prompt, TFunction<void(FString)> Callback, int RetryCount)
{
    FString ApiKey = LoadOpenAIKey();
    if (ApiKey.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("OpenAI API Key is missing!"));
        Callback(TEXT(""));
        return;
    }

    TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL("https://api.openai.com/v1/chat/completions");
    Request->SetVerb("POST");
    Request->SetHeader("Authorization", "Bearer " + ApiKey);
    Request->SetHeader("Content-Type", "application/json");

    FString PostData = FString::Printf(
        TEXT("{ \"model\": \"gpt-4o\", \"messages\": [{ \"role\": \"user\", \"content\": \"%s\" }], \"max_tokens\": 2048 }"),
        *EscapeJSON(Prompt)
    );

    Request->SetContentAsString(PostData);

    UE_LOG(LogTemp, Log, TEXT("OpenAI API Request Sent (Attempt %d): %s"), RetryCount + 1, *PostData);

    Request->OnProcessRequestComplete().BindLambda([Callback, Prompt, RetryCount](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            if (bWasSuccessful && Response.IsValid())
            {
                FString AIResponse = Response->GetContentAsString();
                UE_LOG(LogTemp, Log, TEXT("Raw OpenAI Response: %s"), *AIResponse);

                // JSON 추출 및 정리
                FString CleanedJson = UGodFunction::ExtractValidJson(AIResponse);

                if (!CleanedJson.IsEmpty())
                {
                    Callback(CleanedJson);
                }
                else if (RetryCount < 2)  // 최대 3번까지 재시도
                {
                    UE_LOG(LogTemp, Warning, TEXT("OpenAI 응답이 비어 있음. %d번째 재시도 중..."), RetryCount + 1);
                    FPlatformProcess::Sleep(1.0f);
                    UGodFunction::CallOpenAIAsync(Prompt, Callback, RetryCount + 1); // 자동 재시도
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("OpenAI 응답이 3회 연속 실패. 요청 중단."));
                    Callback(TEXT(""));
                }
            }
            else if (RetryCount < 2)  // 실패 시 최대 3번 재시도
            {
                UE_LOG(LogTemp, Warning, TEXT("OpenAI 요청 실패. %d번째 재시도 중..."), RetryCount + 1);
                FPlatformProcess::Sleep(1.0f);
                UGodFunction::CallOpenAIAsync(Prompt, Callback, RetryCount + 1); // 자동 재시도..
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("OpenAI 요청이 3회 연속 실패. 요청 중단."));
                Callback(TEXT(""));
            }
        });

    Request->ProcessRequest();
}


void UGodFunction::DeleteOldPromptFiles()
{
    FString PromptFolder = FPaths::ProjectSavedDir() + TEXT("Prompt/");

    if (!FPaths::DirectoryExists(PromptFolder))
    {
        UE_LOG(LogTemp, Warning, TEXT("Prompt folder does not exist, skipping deletion."));
        return;
    }

    IFileManager& FileManager = IFileManager::Get();
    // 특정 파일 제외한 파일들만 삭제
    TArray<FString> Files;
    FileManager.FindFiles(Files, *PromptFolder, TEXT(".json"));

    for (const FString& File : Files)
    {
        if (File.Contains("PromptToGod") || File.Contains("PromptToNPC"))
        {
            UE_LOG(LogTemp, Log, TEXT("Skipping essential file: %s"), *File);
            continue; // PromptToGod.json & PromptToNPC.json은 삭제하지 X
        }

        FString FilePath = PromptFolder + File;
        if (FileManager.Delete(*FilePath))
        {
            UE_LOG(LogTemp, Log, TEXT("Deleted old Prompt file: %s"), *FilePath);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to delete file: %s"), *FilePath);
        }
    }
}

void UGodFunction::GeneratePromptWithDelay(UWorld* World, const FString& FileName, const FString& Prompt, float Delay)
{
    if (!World) return;

    FTimerHandle TimerHandle;
    World->GetTimerManager().SetTimer(TimerHandle, [FileName, Prompt]()
        {
            UE_LOG(LogTemp, Log, TEXT("Sending OpenAI API Request for %s"), *FileName);

            CallOpenAIAsync(Prompt, [=](FString AIResponse)
                {
                    if (!AIResponse.IsEmpty()) // 응답이 있을 경우에만 파일 저장
                    {
                        if (UGodFunction::SavePromptToFile(FileName, AIResponse))
                        {
                            UE_LOG(LogTemp, Log, TEXT("Saved AI response for %s"), *FileName);
                        }
                        else
                        {
                            UE_LOG(LogTemp, Error, TEXT("Failed to save %s"), *FileName);
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("OpenAI API returned empty response for %s"), *FileName);
                    }
                });

        }, Delay, false);
}

void UGodFunction::GenerateDefendantPrompt(UWorld* World, TFunction<void()> Callback)
{
    FString FilePath = FPaths::ProjectSavedDir() + TEXT("Prompt/PromptToDefendant.json");

    // ✅ 이미 파일이 존재하면 중복 생성 방지
    if (FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("PromptToDefendant.json 이미 존재하므로 생성하지 않음."));
        if (Callback) Callback();  // NPC 생성으로 넘어감
        return;
    }

    FString PromptToGod = ReadFileContent(FPaths::ProjectSavedDir() + TEXT("Prompt/PromptToGod.json"));
    FString DefendantPrompt = FString::Printf(
        TEXT("{ \"task\": \"피고인 정보를 생성하세요.\", "
            "\"instructions\": ["
            "\"PromptToGod.json을 바탕으로 피고인(NPC)의 정보를 생성하세요.\", "
            "\"npcid 값을 'defendant'로 설정하세요.\"], "
            "\"references\": { \"PromptToGod\": \"%s\" } }"),
        *EscapeJSON(PromptToGod.Mid(0, 2000))
    );

    CallOpenAIAsync(DefendantPrompt, [World, Callback](FString DefendantJson)
        {
            FString CleanedJson = UGodFunction::CleanUpJson(DefendantJson);
            bool bSaved = UGodFunction::SavePromptToFile(TEXT("PromptToDefendant.json"), CleanedJson);

            if (bSaved)
            {
                UE_LOG(LogTemp, Log, TEXT("PromptToDefendant.json 생성 완료!"));
                if (Callback) Callback();  // NPC 생성 시작
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("PromptToDefendant.json 저장 실패!"));
            }
        });
}


void UGodFunction::GenerateNPCPrompts(UWorld* World)
{
    FString PromptToGod = ReadFileContent(FPaths::ProjectSavedDir() + TEXT("Prompt/PromptToGod.json"));
    FString PromptToDefendant = ReadFileContent(FPaths::ProjectSavedDir() + TEXT("Prompt/PromptToDefendant.json"));

    if (PromptToGod.IsEmpty() || PromptToDefendant.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Required prompt files are missing!"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("PromptToDefendant.json 존재 확인됨. NPC 프롬프트 순차적 생성 시작."));

    // 첫 번째 배심원부터 순차적으로 생성 시작
    GenerateJuryNPC(World, 1);
}

// 배심원 NPC 생성 (순차적으로 진행)
void UGodFunction::GenerateJuryNPC(UWorld* World, int JuryIndex)
{
    if (JuryIndex > 3)
    {
        UE_LOG(LogTemp, Log, TEXT("모든 배심원 생성 완료! 이제 주민 생성 시작."));
        GenerateResidentNPC(World, 1);
        return;
    }

    FString PromptToGod = ReadFileContent(FPaths::ProjectSavedDir() + TEXT("Prompt/PromptToGod.json"));
    FString PromptToDefendant = ReadFileContent(FPaths::ProjectSavedDir() + TEXT("Prompt/PromptToDefendant.json"));

    FString JuryPrompt = FString::Printf(
        TEXT("{ \"task\": \"배심원 정보를 생성하세요.\", "
            "\"instructions\": ["
            "\"PromptToGod.json과 PromptToDefendant.json을 참고하여 배심원(NPC) 한 명의 정보를 생성하세요.\", "
            "\"npcid 값을 'jury%03d'로 설정하세요.\", "
            "\"오직 하나의 배심원 정보만 생성해야 합니다.\", "
            "\"JSON 리스트([])가 아닌, 개별적인 JSON 객체를 반환하세요.\"], "
            "\"references\": { \"PromptToGod\": \"%s\", \"PromptToDefendant\": \"%s\" } }"),
        JuryIndex, *EscapeJSON(PromptToGod.Mid(0, 2000)), *EscapeJSON(PromptToDefendant.Mid(0, 2000))
    );

    FString JuryFileName = FString::Printf(TEXT("PromptToJury%d.json"), JuryIndex);
    UE_LOG(LogTemp, Log, TEXT("Generating Jury NPC %d"), JuryIndex);

    CallOpenAIAsync(JuryPrompt, [World, JuryIndex, JuryFileName](FString JuryJson)
        {
            FString CleanJuryJson = UGodFunction::CleanUpJson(JuryJson);
            UGodFunction::SavePromptToFile(JuryFileName, CleanJuryJson);
            GenerateJuryNPC(World, JuryIndex + 1);
        });
}



// 마을 주민 NPC 생성 (순차적으로 진행)
void UGodFunction::GenerateResidentNPC(UWorld* World, int ResidentIndex)
{
    if (ResidentIndex > 5)
    {
        UE_LOG(LogTemp, Log, TEXT("모든 주민 생성 완료!"));
        return;
    }

    FString PromptToGod = ReadFileContent(FPaths::ProjectSavedDir() + TEXT("Prompt/PromptToGod.json"));
    FString PromptToDefendant = ReadFileContent(FPaths::ProjectSavedDir() + TEXT("Prompt/PromptToDefendant.json"));

    FString ResidentPrompt = FString::Printf(
        TEXT("{ \"task\": \"마을 주민 정보를 생성하세요.\", "
            "\"instructions\": ["
            "\"PromptToGod.json과 PromptToDefendant.json을 참고하여 한 명의 마을 주민(NPC) 정보를 생성하세요.\", "
            "\"npcid 값을 'resident%03d'로 설정하세요.\", "
            "\"오직 하나의 주민 데이터만 생성해야 합니다.\", "
            "\"다른 주민의 데이터는 포함하지 마세요.\"], "
            "\"references\": { \"PromptToGod\": \"%s\", \"PromptToDefendant\": \"%s\" } }"),
        ResidentIndex, *EscapeJSON(PromptToGod.Mid(0, 2000)), *EscapeJSON(PromptToDefendant.Mid(0, 2000))
    );

    FString ResidentFileName = FString::Printf(TEXT("PromptToResident%d.json"), ResidentIndex);
    UE_LOG(LogTemp, Log, TEXT("📢 Generating Resident NPC %d"), ResidentIndex);

    CallOpenAIAsync(ResidentPrompt, [World, ResidentIndex, ResidentFileName](FString ResidentJson)
        {
            FString CleanResidentJson = UGodFunction::CleanUpJson(ResidentJson);

            // JSON 응답이 배열([]) 형태라면 첫 번째 객체만 저장
            TSharedPtr<FJsonObject> JsonObject;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(CleanResidentJson);

            if (FJsonSerializer::Deserialize(Reader, JsonObject))
            {
                // `npcid`를 명확하게 지정 (resident001, resident002, ...)
                JsonObject->SetStringField("npcid", FString::Printf(TEXT("resident%03d"), ResidentIndex));

                FString FinalJson;
                TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&FinalJson);
                FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

                CleanResidentJson = FinalJson;
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("OpenAI 응답이 잘못된 형식임. Resident NPC %d 생성 실패. 다음 주민으로 이동."), ResidentIndex);
                GenerateResidentNPC(World, ResidentIndex + 1);
                return;
            }

            // 한 명의 주민 정보만 파일에 저장
            UGodFunction::SavePromptToFile(ResidentFileName, CleanResidentJson);
            UE_LOG(LogTemp, Log, TEXT("Resident NPC %d 생성 완료!"), ResidentIndex);

            // 다음 주민 생성
            GenerateResidentNPC(World, ResidentIndex + 1);
        });
}
