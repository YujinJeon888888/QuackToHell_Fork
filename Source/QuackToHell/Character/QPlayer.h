// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "NPCComponent.h"
#include "QNPC.h"
#include "Player/QPlayerState.h"
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
	/**
	 * @brief Player2N 스피치버블 위젯을 리턴합니다. NPCController에서 접근하기 위함입니다.
	 *
	 * @return Player2Nspeechbubblewidget
	 */
	TObjectPtr<class UQPlayer2NSpeechBubbleWidget> GetPlayer2NSpeechBubbleWidget() const;
public:
	AQPlayer();
	/**
	 * @brief 캐릭터 기준으로 가장 가까이 있는 npc를 반환합니다.
	 * 
	 * @return 캐릭터 기준 가장 가까이 있는 npc
	 */
	TObjectPtr<AActor> GetClosestNPC();

protected:
	/**
	 * @brief Player2NSpeechBubble UI 컴포넌트입니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UWidgetComponent>  Player2NSpeechBubbleWidgetComponent;
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
protected:
	UPROPERTY()
	TObjectPtr<AQPlayerState> LocalPlayerState;
	
	// NPC 대화 관련 check 함수 ------------------------------------------------------
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/** @breif 해당 NPC와 대화가능한지 check*/
	UFUNCTION(Server, Reliable)
	void ServerRPCCanStartConversP2N(AQNPC* NPC);

	/** @breif ServerRPCCanStartConversP2N를 통해 대화시작이 가능한지 체크가 완료된 후 실행되는 클라이언트 RPC
	 * 인자로 시작할 수 있는지 없는지에 대한 bool값이 들어오게 된다. */
	UFUNCTION(Client, Reliable)
	void ClientRPCUpdateCanStartConversP2N(bool bResult);

	/** @brief 해당 NPC와의 대화를 마칠 수 있는 check*/
	UFUNCTION(Server, Reliable)
	void ServerRPCCanFinishConversP2N(AQNPC* NPC);

	/** @breif ServerRPCCanFinishConversP2N를 통해 대화마무리가 가능한지 체크가 완료된 후 실행되는 클라이언트 RPC
	 * 인자로 마무리할 수 있는지 없는지에 대한 bool값이 들어오게 된다. */
	UFUNCTION(Client, Reliable)
	void ClientRPCUpdateCanFinishConversP2N(bool bResult);

	// NPC 대화 관련 대화 실행/마무리 함수 ---------------------------------------------------
	/** @brief NPC와의 대화 시작.*/
	UFUNCTION(Server, Reliable)
	void ServerRPCStartConversation(AQNPC* NPC);
	
	/** @breif ServerRPCStartConversation을 성공적으로 마치게 되면 실행되는 함수. 이 내부에 클라쪽 StartConversation 구현
	* @param NPC 대화대상 npc
	* @param NPCStartResponse NPC 시작 메세지
	*/ 
	UFUNCTION(Client, Reliable)
	void ClientRPCStartConversation(FOpenAIResponse NPCStartResponse, bool bResult);
	
	/** @brief NPC와의 대화 마무리*/
	UFUNCTION(Server, Reliable)
	void ServerRPCFinishConversation(AQNPC* NPC);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCFinishConversation(AQNPC* NPC, bool bResult);

	UFUNCTION(Client, Reliable)
	void ClientRPCGetNPCResponse(FOpenAIResponse NPCStartResponse);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCStartConversation(AQPlayer* Player, AQNPC* NPC);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCFinishConversation(AQPlayer* Player, AQNPC* NPC);
	
private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	/** @brief overlap에 들어온 대상을 담습니다. */
	TArray<TObjectPtr<AActor>> OverlappingNPCs;
private:
	/** @brief 허공말풍선 위젯 클래스 정보를 담습니다. */
	UPROPERTY()
	TObjectPtr<class UQPlayer2NSpeechBubbleWidget> Player2NSpeechBubbleWidget;
};
