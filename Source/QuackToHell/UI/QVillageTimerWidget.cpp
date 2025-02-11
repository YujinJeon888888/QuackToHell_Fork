// Copyright_Team_AriAri


#include "UI/QVillageTimerWidget.h"




void UQVillageTimerWidget::ClinetRPCConverServerTimeToUITime_Implementation(float AccumulatedTime, float MaxTime)
{
	NormalizedTime = AccumulatedTime / MaxTime;
}

float UQVillageTimerWidget::GetNormalizedTime()
{
	return NormalizedTime;
}
