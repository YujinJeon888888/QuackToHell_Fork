// Copyright_Team_AriAri

#include "ResidentComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "JsonObjectConverter.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"

void UResidentComponent::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("UResidentComponent::BeginPlay() 실행됨 - NPC %s"), *NPCID);

    if (NPCID.IsEmpty())
        return;

    // 마을 주민의 JSON 파일 경로 설정
    int32 ResidentIndex = FCString::Atoi(*NPCID) - 2003; // NPCID 2004 -> ResidentIndex 1

    if (ResidentIndex < 1 || ResidentIndex > 5)
        return;

    FString PromptFileName = FString::Printf(TEXT("PromptToResident%d.json"), ResidentIndex);
    PromptFilePath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Prompt"), PromptFileName));

    UE_LOG(LogTemp, Log, TEXT("ResidentComponent - NPC %s는 %s를 사용합니다."), *NPCID, *PromptFilePath);

    // 마을 주민 데이터 로드
    if (!NPCID.IsEmpty())
        LoadPrompt();
}

void UResidentComponent::AskResidentQuestion(const FString& PlayerInput)
{
    if (PlayerInput.IsEmpty() || PlayerInput.Len() < 3)  // 입력 검증 추가
    {
        UE_LOG(LogTemp, Warning, TEXT("Player input is too short or empty."));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Player asked the Resident: %s"), *PlayerInput);
    StartConversation(PlayerInput);
}

void UResidentComponent::StartConversation(const FString& PlayerInput)
{
    if (PromptContent.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Prompt file is empty or failed to load for Resident: %s"), *NPCID);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Player started conversation with %s: %s"), *NPCID, *PlayerInput);

    FOpenAIRequest AIRequest;

    // 첫 대화인지 확인
    bool bIsFirstGreeting = false;
    if (!NPCID.IsEmpty() && P2NDialogueHistory.Contains(NPCID))
    {
        bIsFirstGreeting = P2NDialogueHistory[NPCID].DialogueLines.Num() == 0;
    }

    if (bIsFirstGreeting && PlayerInput.IsEmpty())
    {
        // 첫 대사 생성
        AIRequest.Prompt = FString::Printf(TEXT(
            "아래 설정을 가진 주민이 플레이어를 처음 만났을 때 하는 첫 인사를 생성하세요.\n"
            "==== 주민 설정 ====\n%s\n"
            "첫 인사는 NPC의 성격과 설정을 반영하여 자연스럽게 작성해야 합니다."), *PromptContent);
    }
    else
    {
        // 일반적인 P2N 대화 처리
        AIRequest.Prompt = FString::Printf(TEXT(
            "아래 설정을 가진 주민이 플레이어 '%s'의 질문에 답변합니다.\n"
            "==== 주민 설정 ====\n%s\n"
            "==== 플레이어의 질문 ====\n"
            "플레이어: \"%s\"\n"
            "주민:"), *NPCID, *PromptContent, *PlayerInput);
    }

    AIRequest.MaxTokens = 150;
    AIRequest.SpeakerID = FCString::Atoi(*GetPlayerIDAsString());
    AIRequest.ListenerID = FCString::Atoi(*NPCID);
    AIRequest.ConversationType = EConversationType::P2N;

    RequestOpenAIResponse(AIRequest, [this, PlayerInput](FOpenAIResponse AIResponse)
        {
            ResponseCache.Add(PlayerInput, AIResponse.ResponseText);
            UE_LOG(LogTemp, Log, TEXT("OpenAI Response for NPC %s: %s"), *NPCID, *AIResponse.ResponseText);
            SendNPCResponseToServer(AIResponse.ResponseText);
            SaveP2NDialogue(PlayerInput, AIResponse.ResponseText);
        });
}