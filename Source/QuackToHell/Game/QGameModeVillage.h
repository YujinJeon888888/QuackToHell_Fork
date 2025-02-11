// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "QGameModeVillage.generated.h"

/**
 * 
 */
UCLASS()
class QUACKTOHELL_API AQGameModeVillage : public AGameMode
{
	GENERATED_BODY()

public:
	AQGameModeVillage();

	// ID 관리
	static int32 PlayerIDCount;				const int32 PlayerIDInit = 1000;
	static int32 NPCIDCount;				const int32 NPCIDInit = 2000;
	static int32 EvidenceIDCount;			const int32 EvidenceIDInit = 3000;
	static int32 ConversationIDCount;		const int32 ConversationIDInit = 4000;

	virtual void BeginPlay() override;
	virtual void PreInitializeComponents() override;
};
