// Copyright_Team_AriAri


#include "DefendantComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "JsonObjectConverter.h"

void UDefendantComponent::PerformNPCLogic()
{
    // 피고인 컴포넌트의 로직 시작 로그 출력
    UE_LOG(LogTemp, Log, TEXT("Defendant is preparing their case."));

    // 피고인 JSON 파일 경로 정의
    FString ContentPath = FPaths::ProjectContentDir();
    FString DefendantFilePath = FPaths::Combine(ContentPath, TEXT("DefendantSettings.json"));

    // JSON 파일 내용을 저장할 문자열
    FString DefendantJsonContent;
    if (!FFileHelper::LoadFileToString(DefendantJsonContent, *DefendantFilePath))
    {
        // 파일 읽기 실패 시 로그 출력
        UE_LOG(LogTemp, Error, TEXT("Failed to load Defendant settings file at: %s"), *DefendantFilePath);
        return;
    }

    // JSON 문자열을 파싱하여 JSON 객체로 변환
    TSharedPtr<FJsonObject> DefendantJson;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(DefendantJsonContent);
    if (FJsonSerializer::Deserialize(Reader, DefendantJson) && DefendantJson.IsValid())
    {
        // JSON에서 데이터 추출
        FString Name = DefendantJson->GetStringField("Name");
        FString Role = DefendantJson->GetStringField("Role");
        FString Prompt = DefendantJson->GetStringField("Prompt");

        // 로그로 데이터 확인
        UE_LOG(LogTemp, Log, TEXT("Defendant Name: %s"), *Name);
        UE_LOG(LogTemp, Log, TEXT("Role: %s"), *Role);
        UE_LOG(LogTemp, Log, TEXT("Prompt: %s"), *Prompt);

        // 추출한 데이터를 기반으로 피고인 초기화 로직 구현 가능
        // 예: 특정 속성에 저장하거나 게임 상태에 따라 사용
    }
    else
    {
        // JSON 파싱 실패 시 오류 로그 출력
        UE_LOG(LogTemp, Error, TEXT("Failed to parse Defendant settings JSON file."));
    }

}

void UDefendantComponent::AskDefendantQuestion(const FString& PlayerInput)
{
    UE_LOG(LogTemp, Log, TEXT("Player asked the Defendant: %s"), *PlayerInput);

    // NPCComponent를 통한 OpenAI 요청
    StartConversation(PlayerInput);
}