// Copyright_Team_AriAri


#include "UI/QP2NWidget.h"
#include "Components/TextBlock.h"

void UQP2NWidget::UpdatePlayerText(FString& Text)
{
	PlayerText->SetText(FText::FromString(Text));
}

void UQP2NWidget::UpdateNPCText(FString& Text)
{
	NPCText->SetText(FText::FromString(Text));
}
