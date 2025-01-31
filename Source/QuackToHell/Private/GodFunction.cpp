// Copyright_Team_AriAri


#include "GodFunction.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

FString UGodFunction::LoadPromptData()
{
    FString ContentPath = FPaths::ProjectContentDir();
    FString PromptFilePath = FPaths::Combine(ContentPath, TEXT("Prompt/PromptToGod.json"));

    FString FileContent;
    if (FFileHelper::LoadFileToString(FileContent, *PromptFilePath))
    {
        return FileContent;
    }

    return TEXT("Error: Unable to load PromptToGod.json");
}

void UGodFunction::GenerateNPCDataFromOpenAI()
{
    FString Prompt = LoadPromptData();
    if (Prompt.Contains(TEXT("Error")))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load PromptToGod.json"));
        return;
    }

    // OpenAI API 요청 생성
    TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL("https://api.openai.com/v1/completions");
    Request->SetVerb("POST");
    Request->SetHeader("Authorization", "Bearer YOUR_API_KEY");
    Request->SetHeader("Content-Type", "application/json");

    FString PostData = FString::Printf(TEXT("{ \"prompt\": \"%s\", \"max_tokens\": 2048 }"), *Prompt);
    Request->SetContentAsString(PostData);

    Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            if (bWasSuccessful && Response.IsValid())
            {
                FString AIResponse = Response->GetContentAsString();
                FString SavePath = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Prompts/GeneratedNPCData.json"));

                if (FFileHelper::SaveStringToFile(AIResponse, *SavePath))
                {
                    UE_LOG(LogTemp, Log, TEXT("Generated NPC data saved successfully."));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to save NPC data."));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to generate NPC data from OpenAI."));
            }
        });

    Request->ProcessRequest();
}
