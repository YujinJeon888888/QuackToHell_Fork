// Copyright_Team_AriAri


#include "Character/QNPC.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "QLogCategories.h"
#include "UI/QPlayer2NSpeechBubbleWidget.h"
#include "Components/SphereComponent.h"
#include "NPCComponent.h"
#include "UI/QSpeechBubbleWidget.h"
#include "Net/UnrealNetwork.h"
#include "UI/QNameWidget.h"

AQNPC::AQNPC(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{	/*위치값 세팅*/
	this->GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
	this->GetMesh()->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	/*캡슐 콜라이더 세팅*/
	this->GetCapsuleComponent()->InitCapsuleSize(50.0f, 60.0f);
	/*NPC 컴포넌트*/
	NPCComponent = CreateDefaultSubobject<UNPCComponent>(TEXT("NPCComponent"));
	/*말풍선 UI 컴포넌트*/
	this->SpeechBubbleWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("SpeechBubbleWidget"));
	EWidgetSpace WidgetSpace = EWidgetSpace::Screen;
	this->SpeechBubbleWidgetComponent->SetWidgetSpace(WidgetSpace);
	this->SpeechBubbleWidgetComponent->SetDrawAtDesiredSize(true);
	this->SpeechBubbleWidgetComponent->SetupAttachment(RootComponent);
	TSubclassOf<UQSpeechBubbleWidget> _SpeechBubbleWidget;
	//UQSpeechBubbleWidget을 상속한 클래스만 담을 수 있도록 강제한다.
	this->SpeechBubbleWidgetComponent->SetWidgetClass(_SpeechBubbleWidget);



	/*충돌처리*/
	InteractionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphereComponent->SetupAttachment(RootComponent);
	InteractionSphereComponent->SetSphereRadius(SphereRadius);

	/*충돌처리 바인딩*/
	InteractionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQNPC::OnOverlapBegin);
	InteractionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AQNPC::OnOverlapEnd);
}

// ---------------------------------------------------------------------------------- //



void AQNPC::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// replicate할 프로퍼티 등록
	DOREPLIFETIME(AQNPC, bCanStartConversN2N);
	DOREPLIFETIME(AQNPC, bCanFinishConversN2N);
	DOREPLIFETIME(AQNPC, NPCConversationState);
}

void AQNPC::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AQNPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AQNPC::ServerRPCCanCanStartConversN2N_Implementation(const AQNPC* NPC)
{
}

void AQNPC::ServerRPCCanCanFinishConversN2N_Implementation(const AQNPC* NPC)
{
}


// ---------------------------------------------------------------------------------- //

TObjectPtr<class UQSpeechBubbleWidget> AQNPC::GetSpeechBubbleWidget() const
{
	return SpeechBubbleWidget;
}




bool AQNPC::GetCanStartConversN2N(const AQNPC* NPC)
{
	ServerRPCCanCanStartConversN2N_Implementation(NPC);
	return bCanStartConversN2N;
}

bool AQNPC::GetCanFinishConversN2N(const AQNPC* NPC)
{
	ServerRPCCanCanFinishConversN2N_Implementation(NPC);
	return bCanFinishConversN2N;
}
// ---------------------------------------------------------------------------------- //

void AQNPC::BeginPlay()
{
	Super::BeginPlay();
	/*이름 세팅*/
	FString _Name = (NPCComponent && !NPCComponent->GetNPCName().IsEmpty())
		? NPCComponent->GetNPCName()
		: TEXT("이름비었음");	
	this->SetCharacterName(_Name);
	Super::GetNameWidget()->SetNameWidgetText(GetCharacterName());
	/*말풍선 위젯 변수에 객체값 할당*/
	if (SpeechBubbleWidgetComponent)
	{
		UUserWidget* UserWidget = SpeechBubbleWidgetComponent->GetWidget();
		if (UserWidget)
		{
			SpeechBubbleWidget = Cast<UQSpeechBubbleWidget>(UserWidget);
		}
	}

	/*말풍선 위젯 기본적으로 끈 채로 시작*/
	//SpeechBubbleWidget->TurnOffSpeechBubble();
	

	/*Player2N말풍선 위젯 기본적으로 끈 채로 시작*/
	//Player2NSpeechBubbleWidget->TurnOffSpeechBubble();
}

