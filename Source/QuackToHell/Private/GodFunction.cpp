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
                FString ContentPath = FPaths::ProjectContentDir();

                // 피고인(Defendant) JSON 저장
                FString DefendantPath = FPaths::Combine(ContentPath, TEXT("Prompts/JsonOfDefendant.json"));
                FFileHelper::SaveStringToFile(AIResponse, *DefendantPath);

                // 배심원(Jury) JSON 저장 (3명)
                for (int i = 1; i <= 3; i++)
                {
                    FString JuryPath = FPaths::Combine(ContentPath, FString::Printf(TEXT("Prompts/JsonOfJury%d.json"), i));
                    FFileHelper::SaveStringToFile(AIResponse, *JuryPath);
                }

                // 주민(Resident) JSON 저장 (4명)
                for (int i = 1; i <= 4; i++)
                {
                    FString ResidentPath = FPaths::Combine(ContentPath, FString::Printf(TEXT("Prompts/JsonOfResident%d.json"), i));
                    FFileHelper::SaveStringToFile(AIResponse, *ResidentPath);
                }

                UE_LOG(LogTemp, Log, TEXT("Generated NPC data saved successfully."));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to generate NPC data from OpenAI."));
            }
        });

    Request->ProcessRequest();
}
