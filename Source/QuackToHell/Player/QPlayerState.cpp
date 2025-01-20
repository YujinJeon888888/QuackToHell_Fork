// Copyright_Team_AriAri


#include "Player/QPlayerState.h"
#include "QLogCategories.h"

void AQPlayerState::AddStateTag(FGameplayTag NewStateTag)
{
	PlayerStateTags.AddTag(NewStateTag);
	UE_LOG(LogLogic, Log, TEXT("플레이어 상태 태그 추가: %s"), *NewStateTag.ToString());
}

void AQPlayerState::RemoveStateTag(FGameplayTag StateTag)
{
	PlayerStateTags.RemoveTag(StateTag);
	UE_LOG(LogLogic, Log, TEXT("플레이어 상태 태그 제거: %s"), *StateTag.ToString());
}

bool AQPlayerState::HasStateTag(FGameplayTag StateTag) const
{
	return PlayerStateTags.HasTagExact(StateTag);
}
