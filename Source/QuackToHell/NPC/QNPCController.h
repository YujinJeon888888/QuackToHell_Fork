// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "QNPCController.generated.h"

/**
 * @author 전유진
 * @brief NPCController 클래스입니다.
 */
UCLASS()
class QUACKTOHELL_API AQNPCController : public AAIController
{
	GENERATED_BODY()
protected:
    virtual void BeginPlay() override;

protected:
    /** @brief behavior tree를 지정합니다. */
    UPROPERTY(EditAnyWhere, Category = "NPCBehavior")
    TObjectPtr<UBehaviorTree> BehaviorTree;
    /** @brief blackboard를 지정합니다. */
    UPROPERTY()
    //GC관리를 위해 UPROPERTY()합니다.
    UBlackboardComponent* BlackboardComp;
};
