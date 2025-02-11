// Copyright_Team_AriAri


#include "UI/QVillageTimerWidget.h"

#include "QLogCategories.h"

void UQVillageTimerWidget::UpdateServerTimeToUITime(float AccumulatedTime, const float MaxTime)
{
	if (MaxTime < 0)
	{
		UE_LOG(LogLogic, Log, TEXT("UQVillageTimerWidget UpdateServerTimeToUITime: MaxTime cannot be zero"));
		return;
	}
	UE_LOG(LogLogic, Log, TEXT("LeftTime: %f, MaxTime: %f"), AccumulatedTime, MaxTime);
	NormalizedTime = AccumulatedTime / MaxTime;
}

float UQVillageTimerWidget::GetNormalizedTime()
{
	return NormalizedTime;
}
