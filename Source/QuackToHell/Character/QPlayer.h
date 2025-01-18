// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Character/QCharacter.h"
#include "QPlayer.generated.h"


/**
 * @author 전유진
 * @brief 플레이어 캐릭터 클래스입니다.
 */
UCLASS()
class QUACKTOHELL_API AQPlayer : public AQCharacter
{
	GENERATED_BODY()

public:
	/**
	 * @brief 생성자입니다.
	 */
	AQPlayer();
protected:
	/**
	 * @brief 스프링암 컴포넌트입니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArmComponent;
	/**
	 * @brief 카메라 컴포넌트입니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> CameraComponent;
};
