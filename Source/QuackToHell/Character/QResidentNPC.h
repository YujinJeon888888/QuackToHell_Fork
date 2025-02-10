// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Character/QDynamicNPC.h"
#include "QResidentNPC.generated.h"

/**
 * @author 전유진
 * @brief Resident 캐릭터 클래스입니다.
 */
UCLASS()
class QUACKTOHELL_API AQResidentNPC : public AQDynamicNPC
{
	GENERATED_BODY()
public:
	AQResidentNPC(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void BeginPlay() override;
};
