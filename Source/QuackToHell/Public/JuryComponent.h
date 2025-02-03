// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "NPCComponent.h"
#include "JuryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class QUACKTOHELL_API UJuryComponent : public UNPCComponent
{
    GENERATED_BODY()

public:
    // Override PerformNPCLogic to implement Jury-specific behavior
    virtual void PerformNPCLogic() override;

    /**
     * @brief 배심원이 플레이어의 질문에 대답하도록 NPCComponent에게 요청하는 함수
     * @param PlayerInput 플레이어의 입력 (질문)
     */
    void AskJuryQuestion(const FString& PlayerInput);
};
