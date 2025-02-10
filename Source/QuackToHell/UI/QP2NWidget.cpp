// Copyright_Team_AriAri


#include "UI/QP2NWidget.h"
#include "Components/EditableTextBox.h"
#include "NPC/QNPCController.h"
#include "QLogCategories.h"

#include "Player/QPlayerController.h"
#include "Character/QPlayer.h"
#include "Components/TextBlock.h"
#include "Character/QDynamicNPC.h"
#include "NPC/QDynamicNPCController.h"
#include "NPCComponent.h"
#include "UI/QVillageUIManager.h"

void UQP2NWidget::UpdatePlayerText(const FString& Text)
{
    PlayerText->SetText(FText::FromString(Text));

}

void UQP2NWidget::UpdateNPCText(const FString& Text)
{
    NPCText->SetText(FText::FromString(Text));
}

void UQP2NWidget::SetConversingNPC(const TObjectPtr<class AQDynamicNPCController> NPC)
{
    ConversingNPC = NPC;
    //브로드캐스트 등록
    TObjectPtr<AQDynamicNPC> _NPC = Cast<AQDynamicNPC>(ConversingNPC->GetPawn());
    TObjectPtr<UNPCComponent> NPCComponent = _NPC->GetComponentByClass<UNPCComponent>();
    NPCComponent->OnNPCResponseReceived.AddDynamic(this, &UQP2NWidget::UpdateNPCText);
}

void UQP2NWidget::SetConversingPlayer(const TObjectPtr<class AQPlayerController> Player)
{
    ConversingPlayer = Player;
}

void UQP2NWidget::HandleEnterKeyPress()
{
    /*예외처리*/
    //공백문자열이면 무시한다. 
    if (inputBox->GetText().ToString() == TEXT("")) {
        UE_LOG(LogLogic, Log, TEXT("공백문자열을 input했습니다. 리턴합니다."));
        return;
    }
    ////////////////////////////////////////////////////
    //1 .플레이어 텍스트를 inputbox에 있던거로 업뎃한다.  
    UpdatePlayerText(inputBox->GetText().ToString());
    FString PlayerInput = inputBox->GetText().ToString();

    //2. inputbox는 공란으로 만든다.
    inputBox->SetText(FText::FromString(TEXT("")));
    //3. NPC Text는 음.. 으로 바꾼다.
    UpdateNPCText(WhenGenerateResponseText);
    //4. NPC에게 응답을 요청한다.
    ConversingNPC->Response(PlayerInput);
}



bool UQP2NWidget::IsGerneratingResponse()
{
    if (NPCText->GetText().ToString() == WhenGenerateResponseText) {
        return true;
    }
    return false;
}

void UQP2NWidget::HandleEnterEndButton()
{
    //대화마치기 가능 조건
    //1. 응답생성중이면 안 됨
    //   음... 상태이면 응답생성중이다. 
    bool bGeneratingResponse = IsGerneratingResponse();
    //2. 서버로부터 대화마치기 가능하다고 리턴받아야됨
    TObjectPtr<AQPlayer> _Player = Cast<AQPlayer>(ConversingPlayer->GetPawn());
    TObjectPtr<AQNPC> _NPC = Cast<AQNPC>(ConversingNPC->GetPawn());
    /** @todo 유진 - 이 부분을 Player.cpp에 표시해놓은 부분에서 호출하면 될듯.
     * IsGenerationResponse는 서버에서 확인해주고 있으니까 클라에선 생략 가능*/
    /*
    bool bCanFinishConversing = _Player->GetCanFinishConversP2N(_NPC);
    
    //대화마칠 수 있으면
    if (!bGeneratingResponse && bCanFinishConversing) {
        {
            //1. UI 끈다.
            AQVillageUIManager::GetInstance(GetWorld())->TurnOffUI(EVillageUIType::P2N);
            //2. 대화 끝 처리한다.
            //플레이어
            ConversingPlayer->ConverseEndProcess(Cast<AQNPC>(ConversingNPC->GetPawn()));
            //NPC
            ConversingNPC->EndDialog();
        }
    }
    */
}



