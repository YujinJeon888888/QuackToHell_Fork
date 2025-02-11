// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QVillageTimerWidget.generated.h"

/**
 * @author 전유진
 * @brief 마을 타이머 위젯입니다.
 */
UCLASS()
class QUACKTOHELL_API UQVillageTimerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * @brief 서버에서 누적된 시간을 넣어주면, 함수 내에서 정규화된(0~1사이의값) 값으로 변경한다.
	 * 
	 * @param AccumulatedTime: 0부터 시작해 얼마나 시간이 누적되었는지 전달
	 * @param MaxTime: 타이머가 울려야되는 max값을 전달 (ex. 840초면 타이머 종료)
	 */
	UFUNCTION(Client, Unreliable)
	void ClinetRPCConverServerTimeToUITime(float AccumulatedTime, float MaxTime);
protected:
	/**
	 * @brief 정규화된 시간 값을 리턴한다.
	 * 
	 * @return 정규화된 시간 값(0~1사이의 값)
	 */
	UFUNCTION(BlueprintCallable)
	float GetNormalizedTime();
private:
	float NormalizedTime = 0.f;
};
