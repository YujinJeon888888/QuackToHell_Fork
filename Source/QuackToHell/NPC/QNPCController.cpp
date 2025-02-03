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
    //TODO: 몸멈추기
    
    //TODO: 첫 멘트를 저장 (콜백함수등록)
    //첫멘트 줘 (응답은 브로드캐스트로 옴)
    NPCComponent->StartConversation("");
    //임시코드(원래서버에서킴)
    VillageUIManager->TurnOnUI(EVillageUIType::P2N);
    //텍스트세팅
    //TODO: 텍스트세팅하는 줄 콜백등록해야함
    NPCComponent->OnNPCResponseReceived.AddDynamic(this, &AQNPCController::OnNPCResponseReceived);
    //내 정보 넘겨주기
    Cast<UQP2NWidget>((VillageUIManager->GetActiveWidgets())[EVillageUIType::P2N])->SetConversingNPC(this);
}

void AQNPCController::EndDialog()
{
    /**
     * @todo 구현해야함
     * : 얼음땡
     */
    UE_LOG(LogLogic, Log, TEXT("구현 미완입니다."));
}

void AQNPCController::Response(FString& Text)
{

    //1. 응답 요청
    NPCComponent->StartConversation(Text);
    //2. 콜백함수등록(응답시처리)
    UE_LOG(LogLogic, Log, TEXT("응답요청함수 들어옴: NPCController"));
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

void AQNPCController::OnNPCResponseReceived(const FString& Text)
{
    Cast<UQP2NWidget>((VillageUIManager->GetActiveWidgets())[EVillageUIType::P2N])->UpdateNPCText(Text);
}

//void AQNPCController::Tick(float DeltaTime)
//{
//    if (CachedResponse != NewResponse) {
//        CachedResponse = NewResponse;
//        //TODO: broadcast - 응답 왔어 !
//        OnResponseFinished.Broadcast(NewResponse);
//    }
//}
