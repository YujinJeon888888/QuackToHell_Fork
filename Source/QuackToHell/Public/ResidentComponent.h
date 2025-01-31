// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "NPCComponent.h"
#include "ResidentComponent.generated.h"

/**
 * 
 */
UCLASS()
class QUACKTOHELL_API UResidentComponent : public UNPCComponent
{
	GENERATED_BODY()

public:
	// Override PerformNPCLogic to implement Resident-specific behavior
	virtual void PerformNPCLogic() override;
	
};
