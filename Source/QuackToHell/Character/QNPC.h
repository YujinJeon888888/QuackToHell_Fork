// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Character/QCharacter.h"
#include "QNPC.generated.h"

/**
 * @author 전유진 유서현
 * @brief NPC 캐릭터 클래스입니다.
 */
UCLASS()
class QUACKTOHELL_API AQNPC : public AQCharacter
{
	GENERATED_BODY()

public:
	AQNPC();

private:
	
	UFUNCTION(Server, Reliable)
	bool ServerRPCCanCanStartConversN2N(TObjectPtr<AQNPC> NPC);
};
