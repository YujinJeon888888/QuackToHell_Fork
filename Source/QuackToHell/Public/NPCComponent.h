// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "NPCComponent.generated.h"

/**
 * @author 유서현
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class QUACKTOHELL_API UNPCComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNPCComponent();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// NPC 논리 실행 (자식 클래스에서 오버라이드 가능)
	virtual void PerformNPCLogic();

	// 플레이어가 대화를 시작할 때 호출
	UFUNCTION(BlueprintCallable, Category = "NPC")
	void StartConversation(const FString& PlayerInput);

protected:
	// NPC의 고유 프롬프트 파일 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString PromptFilePath;

	// 로드된 프롬프트 내용
	FString PromptContent;

	// JSON 프롬프트를 로드하는 함수
	void LoadPrompt();

	// OpenAI API에 요청 보내기
	void RequestAIResponse(const FString& PlayerInput);

	// OpenAI API 응답 처리
	void OnAIResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Server RPC 함수
	/** @brief 서버에게 NPC의 시작멘트를 요청한다. ServerRPC 내부에서 ClientRPC를 호출. 클라이언트는 ClientRPC 내부에서 응답 멘트를 저장 */
	UFUNCTION(Server, Reliable)
	void ServerRPCGetGreeting(const FString& NPCID);

	/** @brief 서버에게 플레이어 입력에 대한 NPC의 응답을 요청한다. ServerRPC 내부에서 ClientRPC를 호출. 클라이언트는 ClientRPC 내부에서 응답 멘트를 저장*/
	UFUNCTION(Server, Reliable)
	void ServerRPCGetNPCResponseP2N(const FString& NPCID, const FString& PlayerInput);
	
	/** @brief 서버에게 N2N 대화의 시작멘트를 요청한다. ServerRPC 내부에서 ClientRPC를 호출. 클라이언트는 ClientRPC 내부에서 응답 멘트를 저장*/
	UFUNCTION(Server, Reliable)
	void ServerRPCGetGreetingN2N(const FString& SpeakerNPCID, const FString& ListenerNPCID);

	/** @brief 서버에게 N2N 대화의 NPC 응답을 요청한다. ServerRPC 내부에서 ClientRPC를 호출. 클라이언트는 ClientRPC 내부에서 응답 멘트를 저장 */
	UFUNCTION(Server, Reliable)
	void ServerRPCGetNPCResponseN2N(const FString& SpeakerNPCID, const FString& ListenerNPCID, const FString& NPCInput);

	/** @brief 서버에게 NPC 혼잣말을 생성하도록 요청한다. ServerRPC 내부에서 ClientRPC를 호출. 클라이언트는 ClientRPC 내부에서 응답 멘트를 저장*/
	UFUNCTION(Server, Reliable)
	void ServerRPCGetNPCMonologue(const FString& NPCID);

public:
	// 공용 인터페이스
	/** @brief */
	FString GetNPCResponse(const FString& SpeakerNPCID, const FString& NPCInput, const FString& ListenerNPCID = TEXT("")); 
};
