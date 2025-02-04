// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "QGameModeVillage.generated.h"

/**
 * 
 */
UCLASS()
class QUACKTOHELL_API AQGameModeVillage : public AGameModeBase
{
	GENERATED_BODY()

public:
	AQGameModeVillage();

	// ID 관리
	static int32 PlayerIDCount;				int32 PlayerIDInit = 1000;
	static int32 NPCIDCount;				int32 NPCIDInit = 2000;
	static int32 EvidenceIDCount;			int32 EvidenceIDInit = 3000;
	static int32 ConversationIDCount;		int32 ConversationIDInit = 4000;

	
	
};
