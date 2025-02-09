// Copyright_Team_AriAri


#include "Character/QPlayer.h"

#include "NPCComponent.h"
#include "Camera/CameraComponent.h"
#include "QLogCategories.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "UI/QNameWidget.h"
#include "Character/QNPC.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/QPlayer2NSpeechBubbleWidget.h"
#include "Net/UnrealNetwork.h"
#include "Player/QPlayerState.h"


TObjectPtr<class UQPlayer2NSpeechBubbleWidget> AQPlayer::GetPlayer2NSpeechBubbleWidget() const
{
	return Player2NSpeechBubbleWidget;
}

AQPlayer::AQPlayer()
	:Super()
{
	
	/*캡슐 콜라이더 산하 컴포넌트*/
	RootComponent= this->GetCapsuleComponent();
	//충돌처리
	InteractionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphereComponent->SetupAttachment(RootComponent);
	InteractionSphereComponent->SetSphereRadius(SphereRadius);
	InteractionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQPlayer::OnOverlapBegin);
	InteractionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AQPlayer::OnOverlapEnd);
	//스프링암
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 460.f;
	

	/*스프링암 산하 컴포넌트*/
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	/*회전값 세팅*/
	this->bUseControllerRotationYaw = false;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bInheritRoll = false;
	this->GetCharacterMovement()->bOrientRotationToMovement=true;
	/*위치값 세팅*/
	this->GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -18.f));
	this->GetMesh()->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	/*캡슐 콜라이더 세팅*/
	this->GetCapsuleComponent()->InitCapsuleSize(21.0f, 21.0f);

	/*허공말풍선 UI 컴포넌트*/
	this->Player2NSpeechBubbleWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Player2NSpeechBubbleWidget"));
	EWidgetSpace WidgetSpace = EWidgetSpace::Screen;
	this->Player2NSpeechBubbleWidgetComponent->SetWidgetSpace(WidgetSpace);
	this->Player2NSpeechBubbleWidgetComponent->SetDrawAtDesiredSize(true);
	this->Player2NSpeechBubbleWidgetComponent->SetupAttachment(RootComponent);
	TSubclassOf<UQPlayer2NSpeechBubbleWidget> _Player2NSpeechBubbleWidget;
	//UQPlayer2NSpeechBubbleWidget을 상속한 클래스만 담을 수 있도록 강제한다.
	this->Player2NSpeechBubbleWidgetComponent->SetWidgetClass(_Player2NSpeechBubbleWidget);
}

TObjectPtr<AActor> AQPlayer::GetClosestNPC()
{
	if (OverlappingNPCs.Num() == 0) {
		//대화 대상 없음
		return nullptr;
	}

	TObjectPtr<AActor> ClosestNPC = nullptr;
	float MinDistance = FLT_MAX;

	for (TObjectPtr<AActor> NPC : OverlappingNPCs) {
		//플레이어와 NPC간 거리
		float Distance = FVector::Dist(this->GetActorLocation(), NPC->GetActorLocation());
		//최소거리찾기
		if (Distance < MinDistance) {
			MinDistance = Distance;
			ClosestNPC = NPC;
		}
	}

	return ClosestNPC;
}

void AQPlayer::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogLogic, Log, TEXT("Player의Beginplay호출"));

	LocalPlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AQPlayerState>();
	
	/*이름 세팅*/
	if (LocalPlayerState)
	{
		FString _Name = LocalPlayerState->GetPlayerName();
		this->SetCharacterName(_Name);
		Super::GetNameWidget()->SetNameWidgetText(GetCharacterName());
	}
	
	/*Player2N말풍선 위젯 변수에 객체값 할당*/
	if (Player2NSpeechBubbleWidget)
	{
		UUserWidget* UserWidget = Player2NSpeechBubbleWidgetComponent->GetWidget();
		if (UserWidget)
		{
			Player2NSpeechBubbleWidget = Cast<UQPlayer2NSpeechBubbleWidget>(UserWidget);
		}
	}
	/*Player2N말풍선 위젯 기본적으로 끈 채로 시작*/
	//Player2NSpeechBubbleWidget->TurnOffSpeechBubble();
}

void AQPlayer::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//캐스팅 미성공시 nullptr
	TObjectPtr<AQNPC> OpponentNPC = Cast<AQNPC>(OtherActor);
	if (OpponentNPC) {
		OverlappingNPCs.Add(OtherActor);
	}
}

void AQPlayer::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//캐스팅 미성공시 nullptr
	TObjectPtr<AQNPC> OpponentNPC = Cast<AQNPC>(OtherActor);
	if (OpponentNPC) {
		OverlappingNPCs.Remove(OtherActor);
	}
}

// -------------------------------------------------------------------------------------------------------- //
void AQPlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void AQPlayer::ServerRPCCanStartConversP2N_Implementation(const AQNPC* NPC)
{
	bool bResult = true;
	
	TObjectPtr<UNPCComponent> NPCComponent = NPC->FindComponentByClass<UNPCComponent>();

	if (LocalPlayerState == nullptr || NPCComponent == nullptr)
	{
		bResult = false;
		ClientRPCUpdateCanStartConversP2N_Implementation(bResult);
	}
	
	// OpenAI에 Request를 보낼 수 있는지 확인
	if (!NPCComponent->CanSendOpenAIRequest())
	{
		bResult = false;
	}
	// Player ConversationState가 None인지 확인
	else if (LocalPlayerState->GetPlayerConversationState() != EConversationType::None)
	{
		bResult = false;
	}
	// NPC ConversationState가 None인지 확인
	else if (NPC->GetNPCConversationState() != EConversationType::None)
	{
		bResult = false;
	}
	// NPC와 이전에 대화한 적 있는지 확인
	else if (!NPCComponent->GetIsFirstConversation())
	{
		bResult = false;
	}

	ClientRPCUpdateCanStartConversP2N_Implementation(bResult);
}

void AQPlayer::ClientRPCUpdateCanStartConversP2N_Implementation(bool bCanStartConversP2N)
{
	if (bCanStartConversP2N)
	{
		/** @todo 유진 : 대화를 시작할 수 있을때 클라이언트에서 실행시켜야하는 함수 여기서 호출 */
		
	}
	else
	{
		/** @todo 유진 : 대화를 시작할 수 없을때 클라이언트에서 실행시켜야하는 함수 여기서 호출 */
		
	}
}

void AQPlayer::ServerRPCCanFinishConversP2N_Implementation(const AQNPC* NPC)
{
	bool bResult = true;
	
	TObjectPtr<UNPCComponent> NPCComponent = NPC->FindComponentByClass<UNPCComponent>();

	// Player ConversationState가 None인지 확인
	if (LocalPlayerState->GetPlayerConversationState() == EConversationType::None)
	{
		bResult = false;
	}
	
	// NPC ConversationState가 None인지 확인
	else if (NPC->GetNPCConversationState() == EConversationType::None)
	{
		bResult = false;
	}

	// OpenAI로부터 기다리고 있는 답변이 있는지
	else if (NPCComponent->GetIsRequestInProgress())
	{
		bResult = false;
	}
	
	ClientRPCUpdateCanFinishConversP2N_Implementation(bResult);
}

void AQPlayer::ClientRPCUpdateCanFinishConversP2N_Implementation(bool bCanFinishConversP2N)
{
	if (bCanFinishConversP2N)
	{
		/** @todo 유진 : 대화를 끝낼 수 있을때 클라이언트에서 실행시켜야하는 함수 여기서 호출 */
		
	}
	else
	{
		/** @todo 유진 : 대화를 끝낼 수 없을때 클라이언트에서 실행시켜야하는 함수 여기서 호출 */
		
	}
}

void AQPlayer::ServerRPCStartConversation_Implementation(AQNPC* NPC)
{
	// 상태 업데이트
	LocalPlayerState->SetPlayerConverstationState(EConversationType::P2N);
	NPC->SetNPCConversationState(EConversationType::P2N);

	// OpenAI에게 NPC의 첫 대사 요청하기
	FString Temp = TEXT("");
	FOpenAIRequest Request(
		LocalPlayerState->GetPlayerId(),
		NPC->FindComponentByClass<UNPCComponent>()->GetNPCID(),
		EConversationType::PStart,
		Temp
	);
	NPC->FindComponentByClass<UNPCComponent>()->GetNPCResponse(Request);
}

void AQPlayer::ClientRPCStartConversation_Implementation(FOpenAIResponse NPCStartResponse, bool bResult)
{
	// UE_LOG(LogTemp, Log, TEXT("Player Conversation State Updated. -> %hhd"), LocalPlayerState->GetPlayerConversationState());
	// UE_LOG(LogTemp, Log, TEXT("%s Conversation State Updated. -> %hhd"), *NPC->GetName(), NPC->GetNPCConversationState());
	if (bResult)
	{
		/** @todo 유진 : 서버측에서 대화 시작 로직이 성공적으로 마무리 되었을 떄 실행할 함수 여기서 호출 */
		
	}
	else
	{
		/** @todo 유진 : 서버측에서 대화 시작 로직 실행에 실패했을 때 실행할 함수 여기서 호출*/
		
	}
}

void AQPlayer::ServerRPCFinishConversation_Implementation(AQNPC* NPC)
{
	bool bResult = false;
	// 상태 업데이트
	LocalPlayerState->SetPlayerConverstationState(EConversationType::None);
	NPC->SetNPCConversationState(EConversationType::None);

	ClientRPCFinishConversation_Implementation(NPC, bResult);
}

void AQPlayer::ClientRPCFinishConversation_Implementation(AQNPC* NPC, bool bResult)
{
	UE_LOG(LogTemp, Log, TEXT("Player Conversation State Updated. -> %hhd"), LocalPlayerState->GetPlayerConversationState());
	UE_LOG(LogTemp, Log, TEXT("%s Conversation State Updated. -> %hhd"), *NPC->GetName(), NPC->GetNPCConversationState());
	if (bResult)
	{
		/** @todo 유진 : 서버측에서 대화 마무리 로직이 성공적으로 마무리 되었을 때 실행할 함수 여기서 호출 */
		
	}
	else
	{
		/** @todo 유진 : 서버측에서 대화 마무리 로직 실행에 실패했을 때 실행할 함수 여기서 호출*/
		
	}
}

void AQPlayer::ClientRPCGetNPCResponse_Implementation(FOpenAIResponse NPCStartResponse)
{
	/** @todo 유진 : 서버측에서 NPC응답 왔을 때 실행할 함수 여기서 호출*/
}
