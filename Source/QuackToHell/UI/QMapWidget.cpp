// Copyright_Team_AriAri


#include "UI/QMapWidget.h"
#include "UI/QVillageUIManager.h"

void UQMapWidget::TurnOffMapUI()
{
	AQVillageUIManager::GetInstance(GetWorld())->TurnOffUI(EVillageUIType::Map);
}
