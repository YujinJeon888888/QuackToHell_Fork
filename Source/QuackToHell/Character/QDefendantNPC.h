// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Character/QNPC.h"
#include "QDefendantNPC.generated.h"

/**
 * @author 전유진
 * @brief Defendant 캐릭터 클래스입니다.
 */
UCLASS()
class QUACKTOHELL_API AQDefendantNPC : public AQNPC
{
	GENERATED_BODY()
public:
	AQDefendantNPC(const FObjectInitializer& ObjectInitializer);
protected:
	/**
	 * @brief 모든 컴포넌트 초기화 후 호출 보장.
	 * 
	 */
	virtual void BeginPlay() override;
	/**
	 * @brief 혼잣말멘트.
	 */
	FString TalkMyselfText = TEXT("오늘의 해가 지면 내 재판이 열리겠네요...");

private:

	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) ;

	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) ;
};




