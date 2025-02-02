// Copyright_Team_AriAri


#include "NPC/QNPCController.h"
#include "BehaviorTree/BehaviorTree.h"                // Behavior Tree 클래스
#include "BehaviorTree/BlackboardComponent.h"         // Blackboard 컴포넌트
#include "BehaviorTree/BehaviorTreeComponent.h"       // Behavior Tree 컴포넌트
#include "QLogCategories.h"
#include "NPCComponent.h"
#include "UI/QP2NWidget.h"
#include "UI/QVillageUIManager.h"
#include "Character/QNPC.h"


void AQNPCController::StartDialog()
{
    //TODO: 첫 멘트를 저장
    /*Response = NPCComponent->;*/
    //임시코드(원래서버에서킴)
    VillageUIManager->TurnOnUI(EVillageUIType::P2N);
    //텍스트세팅
    Cast<UQP2NWidget>((VillageUIManager->GetActiveWidgets())[EVillageUIType::P2N])->UpdateNPCText(NewResponse);
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
    //NPCComponent 대입하기
    NPCComponent = Cast<AQNPC>(GetPawn())->FindComponentByClass<UNPCComponent>();
    //VillageManager 대입하기
    VillageUIManager= AQVillageUIManager::GetInstance(GetWorld());
    //TEST
    StartDialog();
}

//void AQNPCController::Tick(float DeltaTime)
//{
//    if (CachedResponse != NewResponse) {
//        CachedResponse = NewResponse;
//        //TODO: broadcast - 응답 왔어 !
//        OnResponseFinished.Broadcast(NewResponse);
//    }
//}
