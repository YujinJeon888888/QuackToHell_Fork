// Copyright_Team_AriAri


#include "NPCComponent.h"
#include "HttpModule.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

// Sets default values for this component's properties
UNPCComponent::UNPCComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UNPCComponent::BeginPlay()
{
	Super::BeginPlay();
	LoadPrompt();

}

void UNPCComponent::LoadPrompt()
{
	if (FFileHelper::LoadFileToString(PromptContent, *PromptFilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("Prompt loaded for NPC: %s"), *PromptContent);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load prompt file: %s"), *PromptFilePath);
	}
}

// NPC 논리 실행 (자식 클래스에서 오버라이드 가능)
void UNPCComponent::PerformNPCLogic()
{
	// Base logic for NPCs (override this in child classes)
}

// 플레이어가 NPC와 대화 시작
void UNPCComponent::StartConversation(const FString& PlayerInput)
{
	UE_LOG(LogTemp, Log, TEXT("Player started conversation: %s"), *PlayerInput);

	// OpenAI API에 요청 보내기
	RequestAIResponse(PlayerInput);
}

// OpenAI API에 요청 보내기
void UNPCComponent::RequestAIResponse(const FString& PlayerInput)
{
	// OpenAI API HTTP 요청 생성
	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL("https://api.openai.com/v1/completions");
	Request->SetVerb("POST");
	Request->SetHeader("Authorization", "Bearer YOUR_API_KEY");  // 🔹 API 키 입력 필수
	Request->SetHeader("Content-Type", "application/json");

	// OpenAI API 요청 JSON 생성
	FString PostData = FString::Printf(TEXT("{ \"prompt\": \"%s\", \"max_tokens\": 50 }"), *PlayerInput);
	Request->SetContentAsString(PostData);

	// 응답을 받으면 OnAIResponseReceived 실행
	Request->OnProcessRequestComplete().BindUObject(this, &UNPCComponent::OnAIResponseReceived);
	Request->ProcessRequest();
}

// OpenAI 응답 처리
void UNPCComponent::OnAIResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		FString AIResponse = Response->GetContentAsString();
		UE_LOG(LogTemp, Log, TEXT("OpenAI Response: %s"), *AIResponse);

		// UI에 메시지 출력 등 추가
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get OpenAI response."));
	}
}

// Called every frame
void UNPCComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// -------------------------------------------------------------------------------------- //

void UNPCComponent::ServerRPCGetGreeting_Implementation(const FString& NPCID)
{
}

void UNPCComponent::ServerRPCGetNPCResponseP2N_Implementation(const FString& NPCID, const FString& PlayerInput)
{
}

void UNPCComponent::ServerRPCGetGreetingN2N_Implementation(const FString& SpeakerNPCID, const FString& ListenerNPCID)
{
}

void UNPCComponent::ServerRPCGetNPCResponseN2N_Implementation(const FString& SpeakerNPCID, const FString& ListenerNPCID,
	const FString& NPCInput)
{
}

void UNPCComponent::ServerRPCGetNPCMonologue_Implementation(const FString& NPCID)
{
}

// ------------------------------------- //

FString UNPCComponent::GetNPCResponse(const FString& SpeakerNPCID, const FString& NPCInput,
	const FString& ListenerNPCID)
{
	FString NPCResponse = TEXT("temp response");
	return NPCResponse;
}