// Copyright_Team_AriAri

#include "JuryComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "JsonObjectConverter.h"

void UJuryComponent::BeginPlay()
{
    Super::BeginPlay();

    // 배심원 ID 가져오기
    FString NPCID = GetOwner()->GetName();

    // NPC ID가 "Jury1", "Jury2", "Jury3"와 매칭되도록 설정
    int32 JuryIndex = 0;

    if (NPCID.Contains(TEXT("Jury001"))) JuryIndex = 1;
    else if (NPCID.Contains(TEXT("Jury002"))) JuryIndex = 2;
    else if (NPCID.Contains(TEXT("Jury003"))) JuryIndex = 3;

    if (JuryIndex > 0)
    {
        PromptFilePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Prompt"), FString::Printf(TEXT("PromptToJury%d.json"), JuryIndex));
        LoadPrompt();
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
        UE_LOG(LogTemp, Error, TEXT("Prompt file is empty or failed to load for Jury: %s"), *GetOwner()->GetName());
        return;
    }

    FString NPCID = GetOwner()->GetName();
    UE_LOG(LogTemp, Log, TEXT("Player started conversation with %s: %s"), *NPCID, *PlayerInput);

    FOpenAIRequest AIRequest;

    // 첫 대화인지 확인
    bool bIsFirstGreeting = !P2NDialogueHistory.Contains(NPCID) || P2NDialogueHistory[NPCID].DialogueLines.Num() == 0;

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
    AIRequest.SpeakerID = "Player";
    AIRequest.ListenerID = NPCID;
    AIRequest.ConversationType = EConversationType::P2N;

    RequestOpenAIResponse(AIRequest, [this, PlayerInput, NPCID](FOpenAIResponse AIResponse)
        {
            ResponseCache.Add(PlayerInput, AIResponse.ResponseText);
            UE_LOG(LogTemp, Log, TEXT("OpenAI Response: %s"), *AIResponse.ResponseText);
            SendNPCResponseToServer(AIResponse.ResponseText);
            SaveP2NDialogue(NPCID, PlayerInput, AIResponse.ResponseText);
        });
}
