// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "QVillageUIManager.generated.h"

/**
 * @author 전유진
 * @brief 마을 내에서의 UIManager입니다. UI를 켜고 끄는 책임을 맡습니다.
 */
UCLASS()
class QUACKTOHELL_API UQVillageUIManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	/**
	 * @brief P2N대화 UI를 켜는 함수입니다.
	 */
	void TurnOnP2NUI();
	/**
	 * @brief P2N대화 UI를 끄는 함수입니다.
	 */
	void TurnOffP2NUI();

};
