// Copyright_Team_AriAri


#include "Game/QGameModeVillage.h"

#include "QVillageGameState.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Player/QPlayerState.h"
#include "UObject/ConstructorHelpers.h"

// 정적(static) 변수 정의
int32 AQGameModeVillage::PlayerIDCount;
int32 AQGameModeVillage::NPCIDCount;
int32 AQGameModeVillage::EvidenceIDCount;
int32 AQGameModeVillage::ConversationIDCount;

AQGameModeVillage::AQGameModeVillage()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerClassRef(TEXT("/Game/Blueprints/Character/BP_QLaywer.BP_QLaywer_C"));
	if (PlayerClassRef.Class) {	// ThirdPersonClassRef.Class가 null이 아니라면
		DefaultPawnClass = PlayerClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerRef(TEXT("/Game/Blueprints/Player/BP_QPlayerController.BP_QPlayerController_C"));
	if (PlayerControllerRef.Class)
	{
		PlayerControllerClass = PlayerControllerRef.Class;
	}
	GameStateClass = AQVillageGameState::StaticClass();
	PlayerStateClass = AQPlayerState::StaticClass();
	
	// 정적(static) 변수는 클래스 차원에서 관리되므로, 생성자에서 값을 설정
	PlayerIDCount = PlayerIDInit;
	NPCIDCount = NPCIDInit;
	EvidenceIDCount = EvidenceIDInit;
	ConversationIDCount = ConversationIDInit;
}
