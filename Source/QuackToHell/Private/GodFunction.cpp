// Copyright_Team_AriAri


#include "GodFunction.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

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
