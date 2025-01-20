// Copyright_Team_AriAri


#include "Player/QPlayerController.h"
#include "QLogCategories.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/QPlayer.h"
#include "Character/QNPC.h"
#include "NPC/QNPCController.h"
#include "Player/QPlayerState.h"
#include "QGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"


void AQPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Player로부터 EnhancedInput subsystem을 가져온다. 
	auto* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (SubSystem) {
		//mapping context를 추가해주는데, priority는 0
		SubSystem->AddMappingContext(InputMappingContext, 0);
	}

	//Player State를 가져온다.
	PlayerState = this->GetPlayerState<AQPlayerState>();
	if (PlayerState == nullptr) {
		UE_LOG(LogLogic, Error, TEXT("플레이어 스테이트를 지정하세요!!"));
	}
}

void AQPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent는 Actor의 멤버변수이다. 멤버변수를 EnhancedInput으로 캐스팅한다.
	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent) {
		/*액션에 대해, 트리거 되면 콜백되는 함수를 바인딩한다.*/
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ThisClass::InputTurn);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &ThisClass::InputInteraction);
		EnhancedInputComponent->BindAction(ConversingQuitAction, ETriggerEvent::Triggered, this, &ThisClass::InputConversingQuit);
		EnhancedInputComponent->BindAction(EnableTurnAction, ETriggerEvent::Started, this, &ThisClass::InputEnableTurn);
		EnhancedInputComponent->BindAction(EnableTurnAction, ETriggerEvent::Completed, this, &ThisClass::InputEnableTurn);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &ThisClass::InputEnableInteracton);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Completed, this, &ThisClass::InputEnableInteracton);
		EnhancedInputComponent->BindAction(ConversingQuitAction, ETriggerEvent::Started, this, &ThisClass::InputEnableConversingQuit);
		EnhancedInputComponent->BindAction(ConversingQuitAction, ETriggerEvent::Completed, this, &ThisClass::InputEnableConversingQuit);

	
	}
}

void AQPlayerController::InputEnableInteracton(const FInputActionValue& InputValue)
{
	bEnableInteraction = InputValue.Get<bool>() ? true : false;
	UE_LOG(LogLogic, Log, TEXT("%s"), InputValue.Get<bool>() ? TEXT("True") : TEXT("False"));
}

void AQPlayerController::InputConversingQuit(const FInputActionValue& InputValue)
{
	//bEnableConversingQuit이 true일때만 상호작용이 가능하다.
	if (bEnableConversingQuit) {
		/** @todo 상호작용 유형별로 나누기 */
		//대화중단해라
		//상태조건: 내가 대화중일 때.
		if (PlayerState->HasStateTag(QGameplayTags::GetTag(EQGameplayTags::Conversing))) {
			this->EndDialog();
		}
	}
}

void AQPlayerController::InputEnableConversingQuit(const FInputActionValue& InputValue)
{
	bEnableConversingQuit = InputValue.Get<bool>() ? true : false;
}

void AQPlayerController::StartDialog()
{
	//0. 대화중이라고 상태전환
	FGameplayTag ConversingTag = QGameplayTags::GetTag(EQGameplayTags::Conversing);
	PlayerState->AddStateTag(ConversingTag);
	UE_LOG(LogLogic, Log, TEXT("플레이어 상태 변경: 대화 중"));

	//1. 상대방 NPC를 불러옴
	TObjectPtr<AQPlayer> _Player = Cast<AQPlayer>(this->GetPawn());
	//2. 상대방 NPC의 컨트롤러를 불러옴
	TObjectPtr<AQNPC> NPC = Cast<AQNPC>(_Player->GetClosestNPC());
	TObjectPtr<AQNPCController> NPCController = Cast<AQNPCController>(NPC->GetController());
	
	//3. 대화 시작하라고 명령한다.
	NPCController->StartDialog();
}

void AQPlayerController::EndDialog()
{
	//0. 대화중의 상태를 remove
	FGameplayTag ConversingTag = QGameplayTags::GetTag(EQGameplayTags::Conversing);
	PlayerState->RemoveStateTag(ConversingTag);
	UE_LOG(LogLogic, Log, TEXT("플레이어 상태 변경: 대화x"));

	//1. 상대방 NPC를 불러옴
	TObjectPtr<AQPlayer> _Player = Cast<AQPlayer>(this->GetPawn());
	//2. 상대방 NPC의 컨트롤러를 불러옴
	TObjectPtr<AQNPC> NPC = Cast<AQNPC>(_Player->GetClosestNPC());
	TObjectPtr<AQNPCController> NPCController = Cast<AQNPCController>(NPC->GetController());

	//3. 대화 그만하라고 명령한다.
	NPCController->EndDialog();
}

void AQPlayerController::InputEnableTurn(const FInputActionValue& InputValue)
{
	bEnableTurn = InputValue.Get<bool>() ? true : false;
}

void AQPlayerController::InputInteraction(const FInputActionValue& InputValue)
{
	//bEnableInteraction이 true일때만 상호작용이 가능하다.
	if (bEnableInteraction) {
		/** @todo 상호작용 유형별로 나누기 */
		//대화시작해라
		//상태조건: 내가 대화중이 아닐 때.
		if (!PlayerState->HasStateTag(QGameplayTags::GetTag(EQGameplayTags::Conversing))) {
			this->StartDialog();
		}
	}
}

void AQPlayerController::InputTurn(const FInputActionValue& InputValue)
{
	//bEnableTurn이 true일때만 화면전환이 가능하다.
	if (bEnableTurn) {
		//내 고개 회전값을 control rotation에 전달. : 상하좌우 고갯짓
		float Yaw = InputValue.Get<FVector2D>().X;
		float Pitch = InputValue.Get<FVector2D>().Y;
		this->AddYawInput(Yaw);
		this->AddPitchInput(-Pitch);
	}
}



void AQPlayerController::InputMove(const FInputActionValue& InputValue)
{
	
	FVector2D MovementVector = InputValue.Get<FVector2D>();

	//앞 이동
	if (MovementVector.X != 0) {
		/*내가 바라보는 곳 기준의 앞 방향을 구해줌*/
		//마우스 방향 회전값 기준
		FRotator Rotator = this->GetControlRotation();
		//해당 회전값의 기준의 앞 방향을 구해준다.
		FVector Direction = UKismetMathLibrary::GetForwardVector(FRotator(0, Rotator.Yaw, 0));
		//방향과 크기를 전달함. (시간마다 거리를 더하는 것은 MovementComponent에서, 속도 세팅은 movementcomponent에서)
		this->GetPawn()->AddMovementInput(Direction, MovementVector.X);
	}
	//옆 이동
	if (MovementVector.Y != 0) {
		/*내가 바라보는 곳 기준의 옆 방향을 구해줌*/
		//마우스 방향 회전값 기준
		FRotator Rotator = this->GetControlRotation();
		//해당 회전값의 기준의 옆 방향을 구해준다.
		FVector Direction = UKismetMathLibrary::GetRightVector(FRotator(0, Rotator.Yaw, 0));
		//방향과 크기를 전달함. (시간마다 거리를 더하는 것은 MovementComponent에서, 속도 세팅은 movementcomponent에서)
		this->GetPawn()->AddMovementInput(Direction, MovementVector.Y);
	}
}


