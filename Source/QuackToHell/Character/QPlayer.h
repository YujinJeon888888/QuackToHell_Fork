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
	AQPlayer();
	/**
	 * @brief 캐릭터 기준으로 가장 가까이 있는 npc를 반환합니다.
	 * 
	 * @return 캐릭터 기준 가장 가까이 있는 npc
	 */
	TObjectPtr<AActor> GetClosestNPC();
protected:
	virtual void BeginPlay() override;
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
	/**
	 * @brief Sphere 컴포넌트입니다. 플레이어를 기준으로 원형을 그려 트리거를 탐지합니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<class USphereComponent> InteractionSphereComponent;
	/**
	 * @brief Sphere 컴포넌트의 반지름입니다. 
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	float SphereRadius = 500.f;
private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	/** @brief overlap에 들어온 대상을 담습니다. */
	TArray<TObjectPtr<AActor>> OverlappingNPCs;

};
