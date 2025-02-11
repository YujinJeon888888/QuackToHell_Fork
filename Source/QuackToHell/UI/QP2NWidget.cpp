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




void UQP2NWidget::HandleEnterEndButton()
{
    //대화마치기 가능한지 서버에게 물어보기
    TObjectPtr<AQPlayer> _Player = Cast<AQPlayer>(ConversingPlayer->GetPawn());
    TObjectPtr<AQNPC> _NPC = Cast<AQNPC>(ConversingNPC->GetPawn());
    
    _Player->ServerRPCCanFinishConversP2N_Implementation(_NPC);    
}

void UQP2NWidget::ClientRPCUpdateCanFinishConversP2N_Implementation(bool bResult)
{
    if (bResult) {
        //1. UI 끈다.
        AQVillageUIManager::GetInstance(GetWorld())->TurnOffUI(EVillageUIType::P2N);
    }
    else {
        UE_LOG(LogLogic, Log, TEXT("대화를 마칠 수 없습니다!"));
    }
}



