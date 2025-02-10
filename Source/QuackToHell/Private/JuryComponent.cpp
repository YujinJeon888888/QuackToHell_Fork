// Copyright_Team_AriAri

#include "JuryComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "JsonObjectConverter.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"

void UJuryComponent::BeginPlay()
{
    Super::BeginPlay();

    if (NPCID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("JuryComponent - NPCID가 설정되지 않았습니다!"));
        return;
    }

    int32 JuryIndex = FCString::Atoi(*NPCID) - 2000; // NPCID 2001 -> JuryIndex 1

    if (JuryIndex < 1 || JuryIndex > 3)
    {
        UE_LOG(LogTemp, Error, TEXT("잘못된 JuryIndex! NPCID: %s"), *NPCID);
        return;
    }

    FString PromptFileName = FString::Printf(TEXT("PromptToJury%d.json"), JuryIndex);
    PromptFilePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Prompt"), PromptFileName);

    LoadPrompt();

    UE_LOG(LogTemp, Log, TEXT("JuryComponent - NPC %s는 %s를 사용합니다."), *NPCID, *PromptFilePath);
}

void UJuryComponent::AskJuryQuestion(const FString& PlayerInput)
{
    UE_LOG(LogTemp, Log, TEXT("Player asked the Jury: %s"), *PlayerInput);
    StartConversation(PlayerInput);
}

void UJuryComponent::StartConversation(const FString& PlayerInput)
{
    if (PromptContent.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Prompt file is empty or failed to load for Jury: %s"), *NPCID);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Player started conversation with %s: %s"), *NPCID, *PlayerInput);

    // 첫 대화인지 확인
    bool bIsFirstGreeting = false;
    if (!NPCID.IsEmpty() && P2NDialogueHistory.Contains(NPCID))
    {
        bIsFirstGreeting = P2NDialogueHistory[NPCID].DialogueLines.Num() == 0;
    }

    FOpenAIRequest AIRequest;

    if (bIsFirstGreeting && PlayerInput.IsEmpty())
    {
        // 첫 대사 생성
        AIRequest.Prompt = FString::Printf(TEXT(
            "아래 설정을 가진 배심원이 플레이어를 처음 만났을 때 하는 첫 인사를 생성하세요.\n"
            "==== 배심원 설정 ====\n%s\n"
            "첫 인사는 NPC의 성격과 설정을 반영하여 자연스럽게 작성해야 합니다."), *PromptContent);
    }
    else
    {
        // 일반적인 P2N 대화 처리
        AIRequest.Prompt = FString::Printf(TEXT(
            "아래 설정을 가진 배심원이 플레이어 '%s'의 질문에 답변합니다.\n"
            "==== 배심원 설정 ====\n%s\n"
            "==== 플레이어의 질문 ====\n"
            "플레이어: \"%s\"\n"
            "배심원:"), *NPCID, *PromptContent, *PlayerInput);
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

