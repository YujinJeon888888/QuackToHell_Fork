// Copyright_Team_AriAri


#include "NPC/QBTTFindPatrolPos.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/KismetMathLibrary.h" // 랜덤 방향 계산에 사용

UQBTTFindPatrolPos::UQBTTFindPatrolPos()
{
	this->NodeName = "Find Patrol Pos";
}

EBTNodeResult::Type UQBTTFindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/*AI Controller 가져오기*/
	TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	if (!AIController) {
		return EBTNodeResult::Failed;
	}

	/*캐릭터 가져오기*/
	TObjectPtr<ACharacter> ControlledCharacter = Cast<ACharacter>(AIController->GetPawn());
	if (!ControlledCharacter) {
		return EBTNodeResult::Failed;
	}

	/*blackboard 가져오기*/
	const TObjectPtr<UBlackboardComponent> BlackboardComp = AIController->GetBlackboardComponent();
	if (!BlackboardComp) {
		return EBTNodeResult::Failed;
	}

	/*원형범위 내에서 랜덤한 patrol 위치 선택*/
	const FVector CurrentLocation = ControlledCharacter->GetActorLocation();
	//순찰반경 반지름
	PatrolRadius = 500.0f;
	//꼭짓점에서 어떤 각도로 갈건지
	const float RandomAngle = FMath::RandRange(0.f, 360.f);
	//라디안으로 변환: FMath::Cos(). FMath::Sin()이 라디안단위를 사용하므로
	const float RadAngle = FMath::DegreesToRadians(RandomAngle);
	//꼭짓점에서 얼마만큼 떨어진 곳으로 갈건지
	const float RandomDistance = FMath::RandRange(MinimumDistance, PatrolRadius);

	/*patrol 위치계산: 방향(각도) * 거리(반지름) = 위치 벡터 */
	const FVector PatrolOffset = FVector
		(FMath::Cos(RadAngle) * RandomDistance, //x
		FMath::Sin(RadAngle) * RandomDistance, //y
		0.0f); // z: 평면 이동이므로 Z축 값은 0으로 고정
	//pawn기준 얼마나 떨어져있는지.
	const FVector NewPatrolLocation = CurrentLocation + PatrolOffset;

	/*블랙보드에 새 patrolpos 설정*/
	BlackboardComp->SetValueAsVector(PatrolPosKey, NewPatrolLocation);

	return EBTNodeResult::Succeeded;
}
