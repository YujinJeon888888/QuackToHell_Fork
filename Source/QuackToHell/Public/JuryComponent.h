// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "NPCComponent.h"
#include "JuryComponent.generated.h"

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class QUACKTOHELL_API UJuryComponent : public UNPCComponent
{
    GENERATED_BODY()

public:
    // Override PerformNPCLogic to implement Jury-specific behavior
    virtual void PerformNPCLogic() override;
};
