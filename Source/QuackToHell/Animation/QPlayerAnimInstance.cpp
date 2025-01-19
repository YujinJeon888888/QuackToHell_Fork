// Copyright_Team_AriAri


#include "Animation/QPlayerAnimInstance.h"
#include "Character/QPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
UQPlayerAnimInstance::UQPlayerAnimInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void UQPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	/*Character, MovementComponent정보를 끌어오자. 업데이트에서는, 플레이어 값을 토대로 애니메이션 재생*/
	Character = Cast<AQPlayer>(TryGetPawnOwner());
	if (Character) {
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UQPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	/*캐릭터, movement 컴포넌트가 널인지 지속적으로 체크한다.*/
	if (Character == nullptr) {
		return;
	}
	if (MovementComponent == nullptr) {
		return;
	}

	/*velocity, groundspeed, bShouldMove, bIsFalling, bIsLanding 를 가져온다.*/
	Velocity = MovementComponent->Velocity;
	//Velocity.Size2D() = 벡터 크기 = 루트(x^2 + y^2)
	GroundSpeed = Velocity.Size2D();
	bShouldMove = (GroundSpeed > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector);
	bIsFalling = MovementComponent->IsFalling();
	bIsLanding = !(MovementComponent->IsFalling())&& MovementComponent->IsMovingOnGround();
}
