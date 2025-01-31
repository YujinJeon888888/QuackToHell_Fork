// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "NPCComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class QUACKTOHELL_API UNPCComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @auther 박시언
	 * @brief NPCComponent의 생성자입니다.
	 */
	UNPCComponent();

protected:
	/**
	 * @auther 박시언
	 * @brief 컴포넌트가 시작될 때 호출됩니다.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * @auther 박시언
	 * @brief NPC의 논리 실행 함수 (자식 클래스에서 오버라이드 가능)
	 */
	virtual void PerformNPCLogic();

	/**
	 * @auther 박시언
	 * @brief 플레이어가 NPC와 대화를 시작할 때 호출됩니다.
	 *
	 * @param PlayerInput 플레이어의 입력 대사
	 */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	void StartConversation(const FString& PlayerInput);

protected:
	/**
	 * @auther 박시언
	 * @brief NPC의 고유 프롬프트 파일 경로 (God이 생성한 JSON 파일의 경로)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString PromptFilePath;

	/**
	 * @auther 박시언
	 * @brief NPC의 성격 정보 (God이 생성한 데이터)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString NPCPersonality;

public:
	/**
	 * @brief NPC의 이름을 반환하는 함수입니다.
	 * @return NPC의 이름 (JSON에서 불러온 값)
	 */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	FString GetNPCName() const;

private:
	/**
	 * @auther 박시언
	 * @brief 첫 대화 여부를 체크하는 변수 (true면 첫 대화)
	 */
	UPROPERTY()
	bool bIsFirstConversation = true;

	/**
	 * @auther 박시언
	 * @brief 로드된 프롬프트 내용을 저장하는 변수
	 */
	FString PromptContent;

	/**
	 * @auther 박시언
	 * @brief NPC의 고유 프롬프트 파일을 로드하는 함수
	 */
	void LoadPrompt();

	/**
	 * @auther 박시언
	 * @brief OpenAI API를 통해 NPC의 첫 인사말을 요청합니다.
	 */
	void RequestGreetingFromAI();

	/**
	 * @auther 박시언
	 * @brief OpenAI API를 통해 플레이어 입력에 대한 NPC 응답을 요청합니다.
	 *
	 * @param PlayerInput 플레이어의 입력 대사
	 */
	void RequestAIResponse(const FString& PlayerInput);

	/**
	 * @auther 박시언
	 * @brief OpenAI API로부터 NPC의 첫 인사말 응답을 처리합니다.
	 *
	 * @param Request HTTP 요청 객체
	 * @param Response HTTP 응답 객체
	 * @param bWasSuccessful 응답 성공 여부
	 */
	void OnGreetingResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/**
	 * @auther 박시언
	 * @brief OpenAI API로부터 NPC의 일반 대화 응답을 처리합니다.
	 *
	 * @param Request HTTP 요청 객체
	 * @param Response HTTP 응답 객체
	 * @param bWasSuccessful 응답 성공 여부
	 */
	void OnAIResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	/**
	 * @auther 박시언
	 * @brief NPC 간 대화를 시작하는 함수입니다. (최대 4턴)
	 *
	 * @param FirstNPCID 첫 번째로 말을 시작하는 NPC의 ID입니다.
	 * @param SecondNPCID 대화를 받는 NPC의 ID입니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	void StartNPCToNPCDialog(const FString& FirstNPCID, const FString& SecondNPCID);

	/**
	 * @auther 박시언
     * @brief NPC 간 대화 응답을 처리하는 함수입니다. (재귀 방식)
     *
     * @param SpeakerNPCID 현재 대화하는 NPC의 ID입니다.
     * @param ListenerNPCID 상대 NPC의 ID입니다.
     * @param ReceivedMessage 상대 NPC가 보낸 대사입니다.
     * @param RemainingTurns 남은 대화 턴 수입니다.
     */
	void ContinueNPCToNPCDialog(
		const FString& SpeakerNPCID,
		const FString& ListenerNPCID,
		const FString& ReceivedMessage,
		int RemainingTurns
	);

protected:
	/**
	 * @auther 박시언
	 * @brief NPC의 첫 인사말을 생성하는 함수입니다.
	 *
	 * @param SpeakerNPCID 대화를 시작하는 NPC의 ID입니다.
	 * @param ListenerNPCID 대화를 듣는 NPC의 ID입니다.
	 * @return FString OpenAI가 생성한 첫 번째 인사말입니다.
	 */
	FString GenerateNPCToNPCGreeting(const FString& SpeakerNPCID, const FString& ListenerNPCID, TFunction<void(FString)> Callback);

	/**
	 * @auther 박시언
	 * @brief NPC가 상대 NPC의 말에 대한 응답을 생성하는 함수입니다.
	 *
	 * @param SpeakerNPCID 대화에 응답하는 NPC의 ID입니다.
	 * @param ListenerNPCID 상대 NPC의 ID입니다.
	 * @param ReceivedMessage 상대 NPC가 보낸 대사입니다.
	 * @return FString OpenAI가 생성한 NPC의 응답 대사입니다.
	 */
	FString GenerateNPCToNPCResponse(const FString& SpeakerNPCID, const FString& ListenerNPCID, const FString& ReceivedMessage, TFunction<void(FString)> Callback);

public:
	/**
	 * @auther 박시언
	 * @brief NPC의 응답을 서버로 보내는 RPC 함수입니다.
	 *
	 * 서버는 이 응답을 받아서 UI 또는 클라이언트로 전달할 수 있습니다.
	 *
	 * @param NPCResponse OpenAI로부터 생성된 NPC의 대사
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void SendNPCResponseToServer(const FString& NPCResponse);

protected:
	/**
	 * @auther 박시언
	 * @brief SendNPCResponseToServer의 실제 구현입니다.
	 *
	 * @param NPCResponse OpenAI로부터 생성된 NPC의 대사
	 */
	void SendNPCResponseToServer_Implementation(const FString& NPCResponse);
	
	/**
	 * @auther 박시언
	 * @brief SendNPCResponseToServer의 유효성 검증 함수입니다.
	 *
	 * @param NPCResponse 전송될 NPC 대사
	 * @return true면 유효한 데이터로 간주
	 */
	bool SendNPCResponseToServer_Validate(const FString& NPCResponse);

public:
	/**
	 * @auther 박시언
	 * @brief NPC가 플레이어와 대화한 대사를 저장하는 변수입니다.
	 */
	UPROPERTY()
	TArray<FString> NPCDialogueHistory;

	/**
	 * @auther 박시언
	 * @brief NPC가 플레이어와 대화한 대사를 저장하는 함수입니다.
	 *
	 * @param NPCResponse NPC가 플레이어에게 한 대사입니다.
	 */
	void SaveNPCDialogue(const FString& NPCResponse);

	/**
	 * @auther 박시언
	 * @brief NPC가 이전 대화를 혼잣말로 변환하는 함수입니다.
	 *
	 * @param Callback 혼잣말을 생성한 후 실행할 함수입니다.
	 * @return FString 기본 혼잣말 (비동기 처리 중 반환)
	 */
	FString GenerateNPCMonologue(TFunction<void(FString)> Callback);

	/**
	 * @auther 박시언
	 * @brief NPC가 혼잣말을 실행하는 함수입니다.
	 */
	void PerformNPCMonologue();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
