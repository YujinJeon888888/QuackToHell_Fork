// Copyright_Team_AriAri

#include "JuryComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "JsonObjectConverter.h"

void UJuryComponent::PerformNPCLogic()
{
    // 배심원 컴포넌트의 로직 시작 로그 출력
    UE_LOG(LogTemp, Log, TEXT("Jury is analyzing arguments and choosing whom to support."));

    // 배심원 JSON 파일 경로 정의 (배심원은 4명)
    FString ContentPath = FPaths::ProjectContentDir();
    for (int32 JuryIndex = 1; JuryIndex <= 4; ++JuryIndex)
    {
        FString JuryFileName = FString::Printf(TEXT("Jury%dSettings.json"), JuryIndex);
        FString JuryFilePath = FPaths::Combine(ContentPath, JuryFileName);

        // JSON 파일 내용을 저장할 문자열
        FString JuryJsonContent;
        if (!FFileHelper::LoadFileToString(JuryJsonContent, *JuryFilePath))
        {
            // 파일 읽기 실패 시 로그 출력
            UE_LOG(LogTemp, Error, TEXT("Failed to load Jury settings file at: %s"), *JuryFilePath);
            continue;
        }

        // JSON 문자열을 파싱하여 JSON 객체로 변환
        TSharedPtr<FJsonObject> JuryJson;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JuryJsonContent);
        if (FJsonSerializer::Deserialize(Reader, JuryJson) && JuryJson.IsValid())
        {
            // JSON에서 데이터 추출
            FString Name = JuryJson->GetStringField("Name");
            FString Role = JuryJson->GetStringField("Role");
            FString Prompt = JuryJson->GetStringField("Prompt");

            // 로그로 데이터 확인
            UE_LOG(LogTemp, Log, TEXT("Jury Name: %s"), *Name);
            UE_LOG(LogTemp, Log, TEXT("Role: %s"), *Role);
            UE_LOG(LogTemp, Log, TEXT("Prompt: %s"), *Prompt);

            // 추출한 데이터를 기반으로 배심원 초기화 로직 구현 가능
            // 예: 특정 속성에 저장하거나 게임 상태에 따라 사용
        }
        else
        {
            // JSON 파싱 실패 시 오류 로그 출력
            UE_LOG(LogTemp, Error, TEXT("Failed to parse Jury settings JSON file: %s"), *JuryFilePath);
        }
    }
}

void UJuryComponent::AskJuryQuestion(const FString& PlayerInput)
{
    UE_LOG(LogTemp, Log, TEXT("Player asked the Jury: %s"), *PlayerInput);

    // NPCComponent를 통한 OpenAI 요청
    StartConversation(PlayerInput);
}