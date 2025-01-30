// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "NPCComponent.h"
#include "DefendantComponent.generated.h"

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class QUACKTOHELL_API UDefendantComponent : public UNPCComponent
{
    GENERATED_BODY()

public:
    // PerformNPCLogic() 함수 선언
    virtual void PerformNPCLogic() override;
};
