// Copyright_Team_AriAri

#include "ResidentComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "JsonObjectConverter.h"

void UResidentComponent::PerformNPCLogic()
{
    // 주민 컴포넌트의 로직 시작 로그 출력
    UE_LOG(LogTemp, Log, TEXT("Resident is providing information about the Defendant's past."));

    // 주민 JSON 파일 경로 정의 (주민은 4명)
    FString ContentPath = FPaths::ProjectContentDir();
    for (int32 ResidentIndex = 1; ResidentIndex <= 4; ++ResidentIndex)
    {
        FString ResidentFileName = FString::Printf(TEXT("Resident%dSettings.json"), ResidentIndex);
        FString ResidentFilePath = FPaths::Combine(ContentPath, ResidentFileName);

        // JSON 파일 내용을 저장할 문자열
        FString ResidentJsonContent;
        if (!FFileHelper::LoadFileToString(ResidentJsonContent, *ResidentFilePath))
        {
            // 파일 읽기 실패 시 로그 출력
            UE_LOG(LogTemp, Error, TEXT("Failed to load Resident settings file at: %s"), *ResidentFilePath);
            continue;
        }

        // JSON 문자열을 파싱하여 JSON 객체로 변환
        TSharedPtr<FJsonObject> ResidentJson;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResidentJsonContent);
        if (FJsonSerializer::Deserialize(Reader, ResidentJson) && ResidentJson.IsValid())
        {
            // JSON에서 데이터 추출
            FString Name = ResidentJson->GetStringField("Name");
            FString Role = ResidentJson->GetStringField("Role");
            FString Prompt = ResidentJson->GetStringField("Prompt");

            // 로그로 데이터 확인
            UE_LOG(LogTemp, Log, TEXT("Resident Name: %s"), *Name);
            UE_LOG(LogTemp, Log, TEXT("Role: %s"), *Role);
            UE_LOG(LogTemp, Log, TEXT("Prompt: %s"), *Prompt);

            // 추출한 데이터를 기반으로 주민 초기화 로직 구현 가능
            // 예: 특정 속성에 저장하거나 게임 상태에 따라 사용
        }
        else
        {
            // JSON 파싱 실패 시 오류 로그 출력
            UE_LOG(LogTemp, Error, TEXT("Failed to parse Resident settings JSON file: %s"), *ResidentFilePath);
        }
    }
}

void UResidentComponent::AskResidentQuestion(const FString& PlayerInput)
{
    UE_LOG(LogTemp, Log, TEXT("Player asked the Resident: %s"), *PlayerInput);

    // NPCComponent를 통한 OpenAI 요청
    StartConversation(PlayerInput);
}