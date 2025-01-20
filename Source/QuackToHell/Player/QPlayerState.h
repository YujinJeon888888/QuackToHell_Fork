// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTagContainer.h" // GameplayTagContainer 사용
#include "QPlayerState.generated.h"


/**
 * @author 전유진
 * @brief player state클래스입니다.
 */

UCLASS()
class QUACKTOHELL_API AQPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	/**
	 * @brief 플레이어의 상태를 관리하는 GameplayTags.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameplayTags")
	FGameplayTagContainer PlayerStateTags;
	/**
	 * @brief 새로운 태그를 추가합니다.
	 * 
	 * @param NewStateTag
	 */
	void AddStateTag(FGameplayTag NewStateTag);
	/**
	 * @brief 태그를 지웁니다.
	 *
	 * @param 지울 태그
	 */
	void RemoveStateTag(FGameplayTag StateTag);
	/**
	 * @brief 태그를 갖고있는지 체크합니다.
	 *
	 * @param 비교할 태그
	 */
	bool HasStateTag(FGameplayTag StateTag) const;
};




