// Copyright_Team_AriAri


#include "Game/QGameModeVillage.h"

// 정적(static) 변수 정의
int32 AQGameModeVillage::PlayerIDCount;
int32 AQGameModeVillage::NPCIDCount;
int32 AQGameModeVillage::EvidenceIDCount;
int32 AQGameModeVillage::ConversationIDCount;

AQGameModeVillage::AQGameModeVillage()
{
	// 정적(static) 변수는 클래스 차원에서 관리되므로, 생성자에서 값을 설정
	PlayerIDCount = PlayerIDInit;
	NPCIDCount = NPCIDInit;
	EvidenceIDCount = EvidenceIDInit;
	ConversationIDCount = ConversationIDInit;
}
