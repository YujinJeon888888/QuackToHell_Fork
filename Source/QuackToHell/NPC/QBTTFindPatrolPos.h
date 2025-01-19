// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "QBTTFindPatrolPos.generated.h"

/**
 * @author 전유진
 * @brief NPC 산책 태스크 정의 클래스입니다.
 */
UCLASS()
class QUACKTOHELL_API UQBTTFindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UQBTTFindPatrolPos();
protected:
	/**
	 * @brief 이 Behavior Tree 태스크의 실행 로직을 정의합니다.
	 * 
	 * @param OwnerComp 이 태스크를 소유하고 있는 Behavior Tree Component에 대한 참조입니다.
	 *                  AI 컨트롤러, 블랙보드, 기타 관련 데이터를 액세스하는 데 사용됩니다.
	 * @param NodeMemory 태스크의 노드 메모리에 대한 포인터입니다.
	 *                   태스크 실행 중 저장해야 하는 태스크별 데이터를 보관하는 데 사용됩니다.
	 *
	 * @return 태스크 실행 결과를 반환합니다.
	 *         - EBTNodeResult::Succeeded: 태스크가 성공적으로 실행되었음을 나타냅니다.
	 *         - EBTNodeResult::Failed: 태스크가 정상적으로 실행되지 않았음을 나타냅니다.
	 *         - EBTNodeResult::InProgress: 태스크 실행이 아직 완료되지 않았음을 나타내며,
	 *           외부에서 완료 신호를 받을 필요가 있습니다.
	 */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	/**
	 * @brief 순찰반경 반지름.
	 */
	UPROPERTY(EditAnywhere)
	float PatrolRadius = 500.0f;
	/**
	 * @brief minimum distance.
	 */
	UPROPERTY(EditAnywhere)
	float MinimumDistance = 500.0f;
	/**
	 * @brief 블랙보드 키 지정
	 */
	UPROPERTY(EditAnywhere)
	FName PatrolPosKey; 
};
