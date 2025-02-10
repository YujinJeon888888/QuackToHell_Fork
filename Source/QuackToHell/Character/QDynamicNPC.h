// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Character/QNPC.h"
#include "QDynamicNPC.generated.h"

/**
 * @author 전유진
 * @brief 스스로 움직이고, 역동적으로 상호작용(n2n, p2n)하는 NPC 캐릭터들을 담습니다.  
 */
UCLASS()
class QUACKTOHELL_API AQDynamicNPC : public AQNPC
{
	GENERATED_BODY()
public:
	AQDynamicNPC(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void BeginPlay() override;
protected:
	/**
	 * @brief E UI 컴포넌트입니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UWidgetComponent> EKeyWidgetComponent;
private:
	/** @brief EKey위젯 클래스 정보를 담습니다. */
	UPROPERTY()
	TObjectPtr<class UQEKeyWidget> EKeyWidget;
	/**
	 * @brief EKey위젯 UI를 켭니다.
	 */
	void TurnOnEKeyUI();
	/**
	 * @brief EKey위젯 UI를 끕니다.
	 */
	void TurnOffEKeyUI();

};




