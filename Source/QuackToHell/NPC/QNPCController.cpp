// Copyright_Team_AriAri


#include "NPC/QNPCController.h"
#include "BehaviorTree/BehaviorTree.h"                // Behavior Tree 클래스
#include "BehaviorTree/BlackboardComponent.h"         // Blackboard 컴포넌트
#include "BehaviorTree/BehaviorTreeComponent.h"       // Behavior Tree 컴포넌트
#include "QLogCategories.h"

void AQNPCController::StartDialog()
{
    /**
     * @todo 구현해야함
     *
     */
    UE_LOG(LogLogic, Log, TEXT("%s: 구현 미완입니다."), *(this->GetPawn()->GetName()));
}
void AQNPCController::EndDialog()
{
    /**
     * @todo 구현해야함
     * 
     */
    UE_LOG(LogLogic, Log, TEXT("구현 미완입니다."));
}

void AQNPCController::Response(FString& Text)
{
}

void AQNPCController::BeginPlay()
{
	Super::BeginPlay();
  
    // Behavior Tree와 Blackboard 컴포넌트가 유효한지 확인
    if (BehaviorTree && BehaviorTree->BlackboardAsset)
    {
        UE_LOG(LogLogic, Log, TEXT("run behavior"));
        // Blackboard 초기화
        UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp);

        // Behavior Tree 실행
        RunBehaviorTree(BehaviorTree);
    }
}
