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

    // NPCID를 int32로 변환
    int32 JuryID = GetNPCID();

    // 배심원 ID 확인 (2001~2003)
    if (JuryID >= 2001 && JuryID <= 2003)
    {
        int32 JuryIndex = JuryID - 2000; // 2001 -> 1, 2002 -> 2, 2003 -> 3
        PromptFilePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Prompt"), FString::Printf(TEXT("PromptToJury%d.json"), JuryIndex));
        LoadPrompt();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("JuryComponent - 잘못된 NPCID (%d). 배심원 ID는 2001~2003이ㅂ니다."), JuryID);
    }
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
    bool bIsFirstGreeting = !P2NDialogueHistory.Contains(NPCID) || P2NDialogueHistory[NPCID].DialogueLines.Num() == 0;

    int32 PlayerID = -1;
    APlayerState* PS = GetOwner() ? Cast<APlayerState>(GetOwner()->GetInstigator()->GetPlayerState()) : nullptr;
    if (PS)
    {
        PlayerID = PS->GetPlayerId();
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
    AIRequest.SpeakerID = PlayerID;
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

