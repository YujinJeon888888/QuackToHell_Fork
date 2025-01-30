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


};
