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
public:
    /**
     * @brief NPC와의 대화시작 요청을 처리하는 함수입니다. npc-npc, player-npc 대화시스템에 활용됩니다.
     * 
     */
    void StartDialog();
    /**
    * @brief NPC와의 대화중단 요청을 처리하는 함수입니다. npc-npc, player-npc 대화시스템에 활용됩니다.
    *
    */
    void EndDialog();
    /**
     * @brief 입력에 대해 대답하라는 요구를 처리합니다.
     * @param query을 인자로 넣습니다.
     */
    void Response(FString& Text);
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
