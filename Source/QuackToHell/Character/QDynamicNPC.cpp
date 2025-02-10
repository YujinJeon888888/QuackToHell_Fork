// Copyright_Team_AriAri


#include "Character/QDynamicNPC.h"
#include "UI/QEKeyWidget.h"
#include "Components/WidgetComponent.h"

AQDynamicNPC::AQDynamicNPC(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	/*EKey UI 컴포넌트*/
	this->EKeyWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EKeyWidget"));
	EWidgetSpace WidgetSpace = EWidgetSpace::Screen;
	this->EKeyWidgetComponent->SetWidgetSpace(WidgetSpace);
	this->EKeyWidgetComponent->SetDrawAtDesiredSize(true);
	this->EKeyWidgetComponent->SetupAttachment(RootComponent);
	TSubclassOf<UQEKeyWidget> _EKeyWidget;
	//QNameWidget을 상속한 클래스만 담을 수 있도록 강제한다.
	this->EKeyWidgetComponent->SetWidgetClass(_EKeyWidget);
}

void AQDynamicNPC::BeginPlay()
{
	Super::BeginPlay();
	
	if (EKeyWidgetComponent)
	{
		UUserWidget* UserWidget = EKeyWidgetComponent->GetWidget();
		if (UserWidget)
		{
			EKeyWidget = Cast<UQEKeyWidget>(UserWidget);
		}
	}
}

void AQDynamicNPC::TurnOnEKeyUI()
{
	EKeyWidget->SetVisibility(ESlateVisibility::Visible);
}

void AQDynamicNPC::TurnOffEKeyUI()
{
	EKeyWidget->SetVisibility(ESlateVisibility::Hidden);
}
