// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Character/QDynamicNPC.h"
#include "QJuryNPC.generated.h"

/**
 * @author 전유진
 * @brief Jury 캐릭터 클래스입니다.
 */
UCLASS()
class QUACKTOHELL_API AQJuryNPC : public AQDynamicNPC
{
	GENERATED_BODY()
public:
	AQJuryNPC(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void BeginPlay() override;
};
