// Copyright_Team_AriAri

#include "DefendantComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "JsonObjectConverter.h"

void UDefendantComponent::BeginPlay()
{
    Super::BeginPlay();

    // 피고인의 JSON 파일 경로 설정
    PromptFilePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Prompt"), TEXT("PromptToDefendant.json"));

    // 피고인 데이터 로드
    LoadPrompt();
}

void UDefendantComponent::PerformNPCLogic()
{
    UE_LOG(LogTemp, Log, TEXT("Defendant is preparing their case."));

    if (PromptContent.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load Defendant's prompt file."));
        return;
    }

    // 피고인의 JSON 데이터를 로그로 출력 (디버깅용)
    UE_LOG(LogTemp, Log, TEXT("Defendant's prompt loaded: %s"), *PromptContent);
}
