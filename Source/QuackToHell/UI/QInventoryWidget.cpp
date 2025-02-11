// Copyright_Team_AriAri


#include "UI/QInventoryWidget.h"
#include "UI/QVillageUIManager.h"
#include "QLogCategories.h"
#include "UI/QRecordWidget.h"
#include "Components/SizeBox.h"

void UQInventoryWidget::TurnOffUI() const
{
	AQVillageUIManager::GetInstance(GetWorld())->TurnOffUI(EVillageUIType::Inventory);
}

void UQInventoryWidget::TurnOnRecordUI() const
{
	//만약 아직 생성되지 않은 상태이면
	if (!AQVillageUIManager::GetInstance(GetWorld())->GetActivedVillageWidgets().Contains(EVillageUIType::Record)) {
		//생성하고
		AQVillageUIManager::GetInstance(GetWorld())->TurnOnUI(EVillageUIType::Record);
		//하위컴포넌트로 달아주기
		InformationBox->AddChild(AQVillageUIManager::GetInstance(GetWorld())->GetActivedVillageWidgets()[EVillageUIType::Record]);
	}
	else {
		//visible로 전환
		AQVillageUIManager::GetInstance(GetWorld())->TurnOnUI(EVillageUIType::Record);
	}
}

void UQInventoryWidget::TurnOffRecordUI() const
{
	AQVillageUIManager::GetInstance(GetWorld())->TurnOffUI(EVillageUIType::Record);
}
