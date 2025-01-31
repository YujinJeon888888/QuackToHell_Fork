// Copyright_Team_AriAri


#include "NPCComponent.h"
#include "HttpModule.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Templates/Function.h"

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

// NPC의 프롬프트 파일 로드
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
	if (bIsFirstConversation)
	{
		// 첫 대화일 경우, OpenAI에게 NPC의 첫 인사 요청
		RequestGreetingFromAI();
		bIsFirstConversation = false;
	}
	else
	{
		// 일반적인 대화 진행
		RequestAIResponse(PlayerInput);
	}
}

// OpenAI API에 첫 인사 요청
void UNPCComponent::RequestGreetingFromAI()
{
	// OpenAI API HTTP 요청 생성
	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL("https://api.openai.com/v1/completions");
	Request->SetVerb("POST");
	Request->SetHeader("Authorization", "Bearer YOUR_API_KEY");
	Request->SetHeader("Content-Type", "application/json");

	// OpenAI API 요청 JSON 생성 (NPC 성격 반영)
	FString Prompt = FString::Printf(
		TEXT("'%s' 성격을 가진 NPC가 상대에게 첫 인사를 건넵니다. 그 첫 문장을 출력하세요."),
		*NPCPersonality
	);

	FString PostData = FString::Printf(TEXT("{ \"prompt\": \"%s\", \"max_tokens\": 50 }"), *Prompt);
	Request->SetContentAsString(PostData);

	// 응답을 받으면 OnGreetingResponseReceived 실행
	Request->OnProcessRequestComplete().BindUObject(this, &UNPCComponent::OnGreetingResponseReceived);
	Request->ProcessRequest();
}

// OpenAI 첫 인사 응답 처리
void UNPCComponent::OnGreetingResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		FString AIResponse = Response->GetContentAsString();
		UE_LOG(LogTemp, Log, TEXT("NPC Greeting Response: %s"), *AIResponse);

		// 첫 인사말을 서버로 전달
		SendNPCResponseToServer(AIResponse);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get NPC greeting response from OpenAI."));
	}
}

// OpenAI API에 일반 대화 요청
void UNPCComponent::RequestAIResponse(const FString& PlayerInput)
{
	// OpenAI API HTTP 요청 생성
	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL("https://api.openai.com/v1/completions");
	Request->SetVerb("POST");
	Request->SetHeader("Authorization", "Bearer YOUR_API_KEY");
	Request->SetHeader("Content-Type", "application/json");

	// OpenAI API 요청 JSON 생성
	FString PostData = FString::Printf(TEXT("{ \"prompt\": \"%s\", \"max_tokens\": 50 }"), *PlayerInput);
	Request->SetContentAsString(PostData);

	// 응답을 받으면 OnAIResponseReceived 실행
	Request->OnProcessRequestComplete().BindUObject(this, &UNPCComponent::OnAIResponseReceived);
	Request->ProcessRequest();
}

// OpenAI 일반 대화 응답 처리
void UNPCComponent::OnAIResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		FString AIResponse = Response->GetContentAsString();
		UE_LOG(LogTemp, Log, TEXT("OpenAI Response: %s"), *AIResponse);

		// 서버로 응답 전달
		SendNPCResponseToServer(AIResponse);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get OpenAI response."));
	}
}

// NPC 간 대화 시작 (콜백 방식 적용)
void UNPCComponent::StartNPCToNPCDialog(const FString& FirstNPCID, const FString& SecondNPCID)
{
	UE_LOG(LogTemp, Log, TEXT("%s와 %s 간 대화 시작!"), *FirstNPCID, *SecondNPCID);

	// 첫 번째 NPC의 인사말 생성 (콜백 사용)
	GenerateNPCToNPCGreeting(FirstNPCID, SecondNPCID, [this, FirstNPCID, SecondNPCID](FString FirstMessage)
		{
			UE_LOG(LogTemp, Log, TEXT("%s: %s"), *FirstNPCID, *FirstMessage);

			// NPC 간 대화 계속 진행 (4턴 중 1턴 완료, 남은 3턴 진행)
			ContinueNPCToNPCDialog(SecondNPCID, FirstNPCID, FirstMessage, 3);
		});
}

// NPC 간 대화 응답 처리 (4턴)
void UNPCComponent::ContinueNPCToNPCDialog(
	const FString& SpeakerNPCID,
	const FString& ListenerNPCID,
	const FString& ReceivedMessage,
	int RemainingTurns)
{
	if (RemainingTurns <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("대화 종료!"));
		return;
	}

	// OpenAI 응답을 비동기 처리
	GenerateNPCToNPCResponse(SpeakerNPCID, ListenerNPCID, ReceivedMessage, [this, SpeakerNPCID, ListenerNPCID, RemainingTurns](FString Response)
		{
			UE_LOG(LogTemp, Log, TEXT("%s: %s"), *SpeakerNPCID, *Response);

			// 대화 역할을 바꾸면서 진행
			ContinueNPCToNPCDialog(ListenerNPCID, SpeakerNPCID, Response, RemainingTurns - 1);
		});
}

// NPC 간 첫 번째 인사말 생성
FString UNPCComponent::GenerateNPCToNPCGreeting(
	const FString& SpeakerNPCID,
	const FString& ListenerNPCID,
	TFunction<void(FString)> Callback)
{
	// 기본 응답값 설정
	FString ResponseText = TEXT("...");

	// OpenAI API 요청 생성
	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL("https://api.openai.com/v1/completions");
	Request->SetVerb("POST");
	Request->SetHeader("Authorization", "Bearer YOUR_API_KEY");
	Request->SetHeader("Content-Type", "application/json");

	// 프롬프트 생성
	FString Prompt = FString::Printf(
		TEXT("'%s' 성격을 가진 NPC가 '%s'에게 첫 인사를 합니다. 자연스러운 첫 마디를 생성하세요."),
		*SpeakerNPCID, *ListenerNPCID
	);

	FString PostData = FString::Printf(TEXT("{ \"prompt\": \"%s\", \"max_tokens\": 50 }"), *Prompt);
	Request->SetContentAsString(PostData);

	// 응답을 받으면 콜백 호출
	Request->OnProcessRequestComplete().BindLambda([Callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			FString ResponseText = TEXT("안녕!");  // 기본값
			if (bWasSuccessful && Response.IsValid())
			{
				ResponseText = Response->GetContentAsString();
			}
			Callback(ResponseText);  // OpenAI 응답을 받은 후 콜백 실행
		});

	Request->ProcessRequest();

	// 비동기 처리 중이므로 기본 응답 반환 (콜백에서 실제 응답 처리)
	return ResponseText;
}

// NPC 간 대화 응답 생성
FString UNPCComponent::GenerateNPCToNPCResponse(
	const FString& SpeakerNPCID,
	const FString& ListenerNPCID,
	const FString& ReceivedMessage,
	TFunction<void(FString)> Callback)
{
	// 기본 응답값 설정
	FString ResponseText = TEXT("...");

	// OpenAI API 요청 생성
	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL("https://api.openai.com/v1/completions");
	Request->SetVerb("POST");
	Request->SetHeader("Authorization", "Bearer YOUR_API_KEY");
	Request->SetHeader("Content-Type", "application/json");

	// 프롬프트 생성 (NPC 성격과 상대방 메시지 반영)
	FString Prompt = FString::Printf(
		TEXT("'%s' 성격을 가진 NPC가 '%s'가 보낸 메시지 '%s'에 대해 대답합니다."),
		*SpeakerNPCID, *ListenerNPCID, *ReceivedMessage
	);

	FString PostData = FString::Printf(TEXT("{ \"prompt\": \"%s\", \"max_tokens\": 50 }"), *Prompt);
	Request->SetContentAsString(PostData);

	// 응답을 받으면 콜백 호출
	Request->OnProcessRequestComplete().BindLambda([Callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			FString ResponseText = TEXT("...");
			if (bWasSuccessful && Response.IsValid())
			{
				ResponseText = Response->GetContentAsString();
			}
			Callback(ResponseText);  // OpenAI 응답을 받은 후 콜백 실행!
		});

	Request->ProcessRequest();

	// 비동기 처리 중이므로 기본 응답 반환 (콜백으로 실제 응답 처리)
	return ResponseText;
}

// NPC가 플레이어와 대화한 내용을 저장
void UNPCComponent::SaveNPCDialogue(const FString& NPCResponse)
{
	// 최대 저장 개수 제한 (예: 최근 5개 대화만 저장)
	if (NPCDialogueHistory.Num() >= 5)
	{
		NPCDialogueHistory.RemoveAt(0); // 가장 오래된 대화 삭제
	}

	NPCDialogueHistory.Add(NPCResponse);
}

// NPC 혼잣말 생성
FString UNPCComponent::GenerateNPCMonologue(TFunction<void(FString)> Callback)
{
	// 최근 대화가 없다면 기본값 반환
	if (NPCDialogueHistory.Num() == 0)
	{
		return TEXT("...");
	}

	// 가장 최근 대화를 가져옴
	FString LastDialogue = NPCDialogueHistory.Last();

	// OpenAI API 요청 생성
	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL("https://api.openai.com/v1/completions");
	Request->SetVerb("POST");
	Request->SetHeader("Authorization", "Bearer YOUR_API_KEY");
	Request->SetHeader("Content-Type", "application/json");

	// 프롬프트 생성 (대화를 혼잣말로 변환)
	FString Prompt = FString::Printf(
		TEXT("NPC가 이전 대화에서 했던 말을 일반적인 혼잣말로 바꿉니다.\n"
			"NPC의 원래 대사: \"%s\"\n"
			"NPC의 혼잣말: "),
		*LastDialogue
	);

	FString PostData = FString::Printf(TEXT("{ \"prompt\": \"%s\", \"max_tokens\": 50 }"), *Prompt);
	Request->SetContentAsString(PostData);

	// 기본 반환값
	FString DefaultMonologue = TEXT("응...?");

	// 응답을 받으면 콜백 실행
	Request->OnProcessRequestComplete().BindLambda([Callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			FString Monologue = TEXT("음...");
			if (bWasSuccessful && Response.IsValid())
			{
				Monologue = Response->GetContentAsString();
			}
			Callback(Monologue); // 콜백 실행
		});

	Request->ProcessRequest();

	return DefaultMonologue; // 기본값 반환
}

// NPC 혼잣말을 실행하는 함수
void UNPCComponent::PerformNPCMonologue()
{
	FString TempMonologue = GenerateNPCMonologue([](FString AIResponse)
		{
			UE_LOG(LogTemp, Log, TEXT("NPC의 혼잣말: %s"), *AIResponse);
		});

	// 기본 반환값 출력 (비동기 처리 중)
	UE_LOG(LogTemp, Log, TEXT("NPC의 혼잣말 (임시): %s"), *TempMonologue);
}

// 서버로 NPC의 응답을 보내는 RPC 함수
void UNPCComponent::SendNPCResponseToServer_Implementation(const FString& NPCResponse)
{
	UE_LOG(LogTemp, Log, TEXT("Sending NPC response to server: %s"), *NPCResponse);

	// 여기서 서버에서 처리하는 로직 추가 (예: UI에 출력, 클라이언트에게 메시지 전달)
	
}

bool UNPCComponent::SendNPCResponseToServer_Validate(const FString& NPCResponse)
{
	return true; // 기본적으로 항상 유효한 메시지라고 가정
}

// Called every frame
void UNPCComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

