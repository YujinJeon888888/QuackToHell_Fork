// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "QPlayerAnimInstance.generated.h"

/**
 * @author 전유진
 * @brief 플레이어 애니메이션 클래스입니다.
 */
UCLASS()
class QUACKTOHELL_API UQPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UQPlayerAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:
	/*플레이어에서 아래 정보들을 끌어오고, 그 정보를 토대로 애니메이션을 재생함*/
	/**
	 * @brief 캐릭터 정보를 끌어옵니다.
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AQPlayer> Character;
	/**
	 * @brief MovementComponent 정보를 끌어옵니다.
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;
protected:
	/**
	 * @brief 속력입니다. groundSpeed 변수대입에 활용합니다.
	 */
	UPROPERTY(BlueprintReadOnly)
	FVector Velocity = FVector::ZeroVector;
	/**
	 * @brief GroundSpeed 입니다. 걷는지 확인하는데 활용됩니다.
	 */
	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed = 0.f;
	/**
	 * @brief Walk중인지 확인합니다. GroundSpeed를 이용해 true/false로 전환합니다.
	 */
	UPROPERTY(BlueprintReadOnly)
	bool bShouldMove = false;
	/**
	 * @brief 떨어지는지 확인합니다.
	 */
	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling = false;
	/**
	 * @brief 착지하는지 확인합니다.
	 */
	UPROPERTY(BlueprintReadOnly)
	bool bIsLanding = false;
private:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
