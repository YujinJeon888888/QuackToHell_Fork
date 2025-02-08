// Copyright_Team_AriAri

#include "GodFunction.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
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

    TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL("https://api.openai.com/v1/completions");
    Request->SetVerb("POST");
    Request->SetHeader("Authorization", "Bearer " + ApiKey);
    Request->SetHeader("Content-Type", "application/json");

    FString PostData = FString::Printf(TEXT("{ \"prompt\": \"%s\", \"max_tokens\": %d }"), *Prompt, 1024);
    Request->SetContentAsString(PostData);

    FString AIResponse = TEXT("");
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
    CompletionEvent->Wait();
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

    // PromptToGod.json 및 PromptToNPC.json 읽기
    FString PromptToGodPath = FPaths::ProjectContentDir() + TEXT("Prompt/PromptToGod.json");
    FString PromptToNPCPath = FPaths::ProjectContentDir() + TEXT("Prompt/PromptToNPC.json");

    FString PromptToGodContent = ReadFileContent(PromptToGodPath);
    FString PromptToNPCContent = ReadFileContent(PromptToNPCPath);

    if (PromptToGodContent.IsEmpty() || PromptToNPCContent.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Missing required prompt files!"));
        return;
    }

    // OpenAI API 호출 및 JSON 파일 생성
    TMap<FString, int> RoleCounts;
    RoleCounts.Add("Defendant", 1);
    RoleCounts.Add("Jury", 3);
    RoleCounts.Add("Resident", 5);

    for (const auto& Role : RoleCounts)
    {
        for (int i = 1; i <= Role.Value; i++)
        {
            FString FileName = FString::Printf(TEXT("PromptTo%s%d.json"), *Role.Key, i);
            FString Prompt = PromptToGodContent + "\n" + PromptToNPCContent;
            FString AIResponse = CallOpenAI(Prompt);

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
