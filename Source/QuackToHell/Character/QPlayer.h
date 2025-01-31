// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "QNPC.h"
#include "Character/QCharacter.h"
#include "QPlayer.generated.h"


/**
 * @author 전유진 유서현
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

private:
	// NPC 대화와 관련된 함수
	/** @breif 해당 NPC가 대화가능한지 check한 후 bool값 반환 */
	UFUNCTION(Server, Reliable)
	bool ServerRPCCanStartConversP2N(TObjectPtr<AQNPC> NPC);

	/** @brief 해당 NPC와의 대화를 마칠 수 있는 check한 후 bool값 반환 */
	UFUNCTION(Server, Reliable)
	bool ServerRPCCanFinishConversP2N(TObjectPtr<AQNPC> NPC);
	
	/** @brief NPC와의 대화 시작.
	 * 혹시라도 예기치 못한 오류로 대화를 시작하지 못하였다면 false 반환
	 */
	UFUNCTION(Server, Reliable)
	bool ServerRPCStartConversation(TObjectPtr<AQNPC> NPC);

	/** @brief NPC와의 대화 마무리
	 * 혹시라도 예기치 못한 오류로 대화를 마치지 못하였다면 false 반환
	 */
	UFUNCTION(Server, Reliable)
	bool ServerRPCFinishConversation(TObjectPtr<AQNPC> NPC);
};
