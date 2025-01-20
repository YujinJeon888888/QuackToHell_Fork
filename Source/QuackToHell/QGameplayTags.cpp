// Copyright_Team_AriAri


#include "QGameplayTags.h"

QGameplayTags::QGameplayTags()
{
}

QGameplayTags::~QGameplayTags()
{
}

const FGameplayTag QGameplayTags::GetTag(EQGameplayTags EnumValue)
{
    switch (EnumValue)
    {
    case EQGameplayTags::Conversing:
        return FGameplayTag::RequestGameplayTag(FName("State.Conversing"));
    default:
        return FGameplayTag();
    }
}
