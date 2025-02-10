// Copyright_Team_AriAri


#include "Character/QDefendantNPC.h"
#include "UI/QNameWidget.h"
#include "Math/Color.h"
#include "Character/QPlayer.h"
#include "UI/QSpeechBubbleWidget.h"
#include "NPCComponent.h"
#include "DefendantComponent.h"



AQDefendantNPC::AQDefendantNPC(const FObjectInitializer& ObjectInitializer)
	: Super(
		ObjectInitializer.SetDefaultSubobjectClass<UDefendantComponent>(TEXT("NPCComponent"))
	)
{
}

void AQDefendantNPC::BeginPlay()
{
	Super::BeginPlay();
	/*이름표 세팅*/
	//구조체이므로 스택에 할당.
	FLinearColor MyColor;
	MyColor.R = 1.0f;
	MyColor.G = 0.835f;
	MyColor.B = 0.0f;
	MyColor.A = 1.0f;
	//노란색
	Super::GetNameWidget()->SetColorAndOpacity(MyColor);
}

void AQDefendantNPC::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<AQPlayer> OpponentPawn = Cast<AQPlayer>(OtherActor);
	if (OpponentPawn) {
		//1. 말풍선킨다.
		GetSpeechBubbleWidget()->TurnOnSpeechBubble();
		//2. 혼잣말한다. 
		GetSpeechBubbleWidget()->UpdateText(TalkMyselfText);
	}
}

void AQDefendantNPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TObjectPtr<AQPlayer> OpponentPawn = Cast<AQPlayer>(OtherActor);
	if (OpponentPawn) {
		//3. 말풍선끈다.
		Super::GetSpeechBubbleWidget()->TurnOffSpeechBubble();
	}
}
