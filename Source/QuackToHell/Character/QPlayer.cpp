// Copyright_Team_AriAri


#include "Character/QPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AQPlayer::AQPlayer()
	:Super()
{
	/*캡슐 콜라이더 산하 컴포넌트*/
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(this->GetCapsuleComponent());
	SpringArmComponent->SetWorldRotation(FRotator(0.f, -30.f, 0.f));
	SpringArmComponent->TargetArmLength = 700.f;
	/*스프링암 산하 컴포넌트*/
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	/*회전값 세팅*/
	this->bUseControllerRotationYaw = false;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bInheritRoll = false;
	this->GetCharacterMovement()->bOrientRotationToMovement=true;
}
