// Copyright_Team_AriAri


#include "GodFunction.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

// OpenAI API 호출 함수
FString UGodFunction::CallOpenAI(const FString& Prompt)
{
    FString ApiKey;
    GConfig->GetString(TEXT("OpenAI"), TEXT("ApiKey"), ApiKey, GGameIni);

    if (ApiKey.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("OpenAI API Key is missing! Check DefaultGame.ini"));
        return TEXT("");
    }

    // API 요청 생성
    TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL("https://api.openai.com/v1/completions");
    Request->SetVerb("POST");
    Request->SetHeader("Authorization", "Bearer " + ApiKey);
    Request->SetHeader("Content-Type", "application/json");

    FString PostData = FString::Printf(TEXT("{ \"prompt\": \"%s\", \"max_tokens\": 1024 }"), *Prompt);
    Request->SetContentAsString(PostData);

    FString AIResponse = TEXT("");

    // 비동기 요청을 동기적으로 처리하기 위한 람다 함수
    FEvent* CompletionEvent = FPlatformProcess::GetSynchEventFromPool(true);
    Request->OnProcessRequestComplete().BindLambda([&AIResponse, CompletionEvent](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            if (bWasSuccessful && Response.IsValid() && Response->GetResponseCode() == 200)
            {
                AIResponse = Response->GetContentAsString();
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("OpenAI API Request Failed! Response Code: %d"), Response->GetResponseCode());
            }
            CompletionEvent->Trigger();
        });

    Request->ProcessRequest();
    CompletionEvent->Wait();  // 요청 완료될 때까지 대기
    FPlatformProcess::ReturnSynchEventToPool(CompletionEvent);

    return AIResponse;
}

// JSON 파일 저장 함수
bool UGodFunction::SavePromptToFile(const FString& FileName, const FString& Content)
{
    FString FilePath = FPaths::ProjectContentDir() + TEXT("Prompt/") + FileName;
    return FFileHelper::SaveStringToFile(Content, *FilePath);
}

// NPC 프롬프트 생성 함수
void UGodFunction::GenerateNPCPrompts()
{
    UE_LOG(LogTemp, Log, TEXT("Generating NPC Prompts..."));

    // 역할별 OpenAI 프롬프트 설정
    TMap<FString, FString> RolePrompts;
    RolePrompts.Add("Defendant", "Generate a detailed backstory for a defendant in a supernatural court trial.");
    RolePrompts.Add("Jury", "Create a unique jury member with personality traits, background, and opinion on the case.");
    RolePrompts.Add("Resident", "Generate a town resident who witnessed something unusual related to the case.");

    // 파일 이름 패턴 정의
    TMap<FString, FString> RoleFileNames;
    RoleFileNames.Add("Defendant", "PromptToNPC.json");
    RoleFileNames.Add("Jury", "PromptToJury%d.json");
    RoleFileNames.Add("Resident", "PromptToResident%d.json");

    // 역할별로 API 요청 생성 및 실행
    for (const auto& RolePrompt : RolePrompts)
    {
        FString Role = RolePrompt.Key;
        FString PromptTemplate = RolePrompt.Value;

        int NPCCount = (Role == "Defendant") ? 1 : (Role == "Jury" ? 3 : 5);

        for (int i = 1; i <= NPCCount; i++)
        {
            FString FileName = (Role == "Defendant") ? RoleFileNames[Role] : FString::Printf(*RoleFileNames[Role], i);

            UE_LOG(LogTemp, Log, TEXT("Requesting OpenAI for: %s"), *FileName);

            FString AIResponse = CallOpenAI(PromptTemplate);

            if (!AIResponse.IsEmpty())
            {
                if (SavePromptToFile(FileName, AIResponse))
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
        }
    }

    UE_LOG(LogTemp, Log, TEXT("All NPC Prompts Generated!"));
}

