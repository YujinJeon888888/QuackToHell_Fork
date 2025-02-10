// Copyright_Team_AriAri


#include "Character/QDynamicNPC.h"
#include "UI/QEKeyWidget.h"
#include "Character/QPlayer.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "QLogCategories.h"
#include "UI/QPlayer2NSpeechBubbleWidget.h"
AQDynamicNPC::AQDynamicNPC(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	/*허공말풍선 UI 컴포넌트*/
	EWidgetSpace WidgetSpace = EWidgetSpace::Screen;
	this->Player2NSpeechBubbleWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Player2NSpeechBubbleWidget"));
	this->Player2NSpeechBubbleWidgetComponent->SetWidgetSpace(WidgetSpace);
	this->Player2NSpeechBubbleWidgetComponent->SetDrawAtDesiredSize(true);
	this->Player2NSpeechBubbleWidgetComponent->SetupAttachment(RootComponent);
	TSubclassOf<UQPlayer2NSpeechBubbleWidget> _Player2NSpeechBubbleWidget;
	//UQPlayer2NSpeechBubbleWidget을 상속한 클래스만 담을 수 있도록 강제한다.
	this->Player2NSpeechBubbleWidgetComponent->SetWidgetClass(_Player2NSpeechBubbleWidget);

	/*EKey UI 컴포넌트*/
	this->EKeyWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EKeyWidget"));
	this->EKeyWidgetComponent->SetWidgetSpace(WidgetSpace);
	this->EKeyWidgetComponent->SetDrawAtDesiredSize(true);
	this->EKeyWidgetComponent->SetupAttachment(RootComponent);
	TSubclassOf<UQEKeyWidget> _EKeyWidget;
	//QNameWidget을 상속한 클래스만 담을 수 있도록 강제한다.
	this->EKeyWidgetComponent->SetWidgetClass(_EKeyWidget);

}
TObjectPtr<AActor> AQDynamicNPC::GetClosestNPC()
{
	if (OverlappingNPCs.Num() == 0) {
		//대화 대상 없음
		return nullptr;
	}

	TObjectPtr<AActor> ClosestNPC = nullptr;
	float MinDistance = FLT_MAX;

	for (TObjectPtr<AActor> NPC : OverlappingNPCs) {
		//캐릭터와 NPC간 거리
		float Distance = FVector::Dist(this->GetActorLocation(), NPC->GetActorLocation());
		//최소거리찾기
		if (Distance < MinDistance) {
			MinDistance = Distance;
			ClosestNPC = NPC;
		}
	}

	return ClosestNPC;
}


TObjectPtr<class UQPlayer2NSpeechBubbleWidget> AQDynamicNPC::GetPlayer2NSpeechBubbleWidget() const
{
	return Player2NSpeechBubbleWidget;
}

void AQDynamicNPC::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//만약 플레이어가 다가왔을 시, E대화하기 유아이 띄우기
	TObjectPtr<AQPlayer> OpponentPlayer = Cast<AQPlayer>(OtherActor);
	if (OpponentPlayer) {
		TurnOnEKeyUI();
	}
	

	//캐스팅 미성공시 nullptr
	TObjectPtr<AQDynamicNPC> OpponentNPC = Cast<AQDynamicNPC>(OtherActor);
	if (OpponentNPC) {
		OverlappingNPCs.Add(OpponentNPC);
	}
}

void AQDynamicNPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//만약 플레이어가 나갔을 시, E대화하기 유아이 끄기
	TObjectPtr<AQPlayer> OpponentPlayer = Cast<AQPlayer>(OtherActor);
	if (OpponentPlayer) {
		TurnOffEKeyUI();
	}
	
	
	//캐스팅 미성공시 nullptr
	TObjectPtr<AQDynamicNPC> OpponentNPC = Cast<AQDynamicNPC>(OtherActor);
	if (OpponentNPC) {
		OverlappingNPCs.Remove(OpponentNPC);
	}
}
void AQDynamicNPC::BeginPlay()
{
	Super::BeginPlay();
	/*EKey 위젯 변수에 객체값 할당*/
	if (EKeyWidgetComponent)
	{
		UUserWidget* UserWidget = EKeyWidgetComponent->GetWidget();
		if (UserWidget)
		{
			EKeyWidget = Cast<UQEKeyWidget>(UserWidget);
		}
	}

	/*Player2N말풍선 위젯 변수에 객체값 할당*/
	if (Player2NSpeechBubbleWidgetComponent)
	{
		UUserWidget* UserWidget = Player2NSpeechBubbleWidgetComponent->GetWidget();
		if (UserWidget)
		{
			Player2NSpeechBubbleWidget = Cast<UQPlayer2NSpeechBubbleWidget>(UserWidget);
		}
	}
}

void AQDynamicNPC::TurnOnEKeyUI()
{
	UE_LOG(LogLogic, Log, TEXT("UI켜기시도"));
	EKeyWidget->SetVisibility(ESlateVisibility::Visible);
}

void AQDynamicNPC::TurnOffEKeyUI()
{
	UE_LOG(LogLogic, Log, TEXT("UI끄기시도"));
	EKeyWidget->SetVisibility(ESlateVisibility::Hidden);
}
