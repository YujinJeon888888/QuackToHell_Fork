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
private:
	/**
	 * @brief 상호작용 입력 처리 함수입니다..
	 *
	 * \param InputValue 입력 값
	 */
	void InputInteracton(const FInputActionValue& InputValue);
	/**
	 * @brief 이동 입력 처리 함수입니다..
	 *
	 * \param InputValue 입력 값
	 */
	void InputMove(const FInputActionValue& InputValue);
	/**
	 * @brief 회전 입력 처리 함수입니다..
	 *
	 * \param InputValue 입력 값
	 */
	void InputTurn(const FInputActionValue& InputValue);
	/**
	 * @brief 회전 활성화 처리 함수입니다.
	 *
	 * \param InputValue 입력 값
	 */
	void InputEnableTurn(const FInputActionValue& InputValue);
	/**
	 * @brief 회전 활성화 처리 함수함수에서 true가 들어올 시, 이 변수도 true됩니다.
	 */
	bool bEnableTurn = false;
};
