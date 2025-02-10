// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "QNPCController.generated.h"


/** @brief 대화 유형 */
UENUM()
enum class ENPCConversationType :uint8 {
    P2N = 0,
    N2N,

};

/**
 * @author 전유진
 * @brief NPCController 클래스입니다.
 */
UCLASS()
class QUACKTOHELL_API AQNPCController : public AAIController
{
    GENERATED_BODY()

};




