// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/QCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UObject/SoftObjectPath.h"
#include "UI/QNameWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values
AQCharacter::AQCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*이름표 UI 컴포넌트*/
	this->WidgetComponent= CreateDefaultSubobject<UWidgetComponent>(TEXT("NameWidget"));
	EWidgetSpace WidgetSpace = EWidgetSpace::Screen;
	this->WidgetComponent->SetWidgetSpace(WidgetSpace);
	this->WidgetComponent->SetDrawAtDesiredSize(true);
	this->WidgetComponent->SetupAttachment(RootComponent);
	TSubclassOf<UQNameWidget> _NameWidget;
	//QNameWidget을 상속한 클래스만 담을 수 있도록 강제한다.
	this->WidgetComponent->SetWidgetClass(_NameWidget);
}

// Called when the game starts or when spawned
void AQCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetComponent)
	{
		UUserWidget* UserWidget = WidgetComponent->GetWidget();
		if (UserWidget)
		{
			NameWidget = Cast<UQNameWidget>(UserWidget);
		}
	}
}

const FString& AQCharacter::GetCharacterName() const
{
	return CharacterName;	
}

void AQCharacter::SetCharacterName(FString& Name)
{
	CharacterName = Name;
}



// Called every frame
void AQCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AQCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

