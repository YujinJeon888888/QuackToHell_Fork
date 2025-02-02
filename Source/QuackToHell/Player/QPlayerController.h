// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QPlayerController.generated.h"
struct FInputActionValue;
/**
 * @author 전유진
 * @brief 플레이어 컨트롤러 클래스입니다.
 */
UCLASS()
class QUACKTOHELL_API AQPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
protected:
	/** @brief IMC입니다. */
	UPROPERTY(EditAnyWhere, Category = "Input")
	TObjectPtr<class UInputMappingContext> InputMappingContext;
	/** @brief Interaction IA 입니다. */
	UPROPERTY(EditAnyWhere, Category = "Input")
	TObjectPtr<class UInputAction> InteractionAction;
	/** @brief Move IA 입니다. */
	UPROPERTY(EditAnyWhere, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;
	/** @brief Turn IA 입니다. */
	UPROPERTY(EditAnyWhere, Category = "Input")
	TObjectPtr<class UInputAction> TurnAction;
	/** @brief Enable Turn IA 입니다. true일 경우에만 turn이 활성화됩니다. */
	UPROPERTY(EditAnyWhere, Category = "Input")
	TObjectPtr<class UInputAction> EnableTurnAction;
	/** @brief ConversingQuit IA 입니다. true 시 대화가 중단됩니다. */
	UPROPERTY(EditAnyWhere, Category = "Input")
	TObjectPtr<class UInputAction> ConversingQuitAction;
private:
	/**
	 * @brief 이동 입력 처리 함수입니다..
	 *
	 * @param InputValue 입력 값
	 */
	void InputMove(const FInputActionValue& InputValue);
	/**
	 * @brief 회전 입력 처리 함수입니다..
	 *
	 * @param InputValue 입력 값
	 */
	void InputTurn(const FInputActionValue& InputValue);
	/**
	 * @brief 회전 활성화 처리 함수입니다.
	 *
	 * @param InputValue 입력 값
	 */
	void InputEnableTurn(const FInputActionValue& InputValue);
	/**
	 * @brief 상호작용 입력 처리 함수입니다..
	 *
	 * @param InputValue 입력 값
	 */
	void InputInteraction(const FInputActionValue& InputValue);

	/**
	 * @brief 상호작용 활성화 처리 함수입니다.
	 *
	 * @param InputValue 입력 값
	 */
	void InputEnableInteracton(const FInputActionValue& InputValue);
	
	/**
	 * @brief 대화중단 입력 처리 함수입니다..
	 *
	 * @param InputValue 입력 값
	 */
	void InputConversingQuit(const FInputActionValue& InputValue);
	/**
	 * @brief 대화중단 활성화 처리 함수입니다.
	 *
	 * @param InputValue 입력 값
	 */
	void InputEnableConversingQuit(const FInputActionValue& InputValue);
	
	
	/**
	 * @brief 회전 활성화 처리 함수함수에서 true가 들어올 시, 이 변수도 true됩니다.
	 */
	bool bEnableTurn = false;
	/**
	 * @brief 상호작용 활성화 처리 함수함수에서 true가 들어올 시, 이 변수도 true됩니다.
	 */
	bool bEnableInteraction = false;
	/**
	 * @brief 대화중단 활성화 처리 함수함수에서 true가 들어올 시, 이 변수도 true됩니다.
	 */
	bool bEnableConversingQuit = false;
	/**
	 * @brief 대화를 시작합니다.
	 */
	void StartDialog();
	/**
	 * @brief 대화를 중단합니다.
	 */
	void EndDialog();
	/**
	 * @brief 공백말풍선 UI컴포넌트를 킵니다.
	 */
	void TurnOnPlayer2NSpeechBubble();
	/**
	 * @brief 공백말풍선 UI컴포넌트를 끕니다.
	 */
	void TurnOffPlayer2NSpeechBubble();
	/**
	 * @brief 몸을 멈춥니다.
	 */
	void FreezePawn();
	
	/**
	 * @brief 몸을 얼음땡 합니다.
	 */
	void UnFreezePawn();
	/**
	 * @brief 대화중일때의 처리를 구현합니다. 1. 몸 멈추기 2. 대화상태로 전환하기
	 */
	void ConverseProcess(TObjectPtr<class AQNPC> NPC);

};




