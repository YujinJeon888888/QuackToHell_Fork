// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
/**
 * @brief FGameplayTag는 Unreal의 메타데이터 시스템과 강하게 연결되어 있기 때문에 전방 선언으로 사용할 수 없다고 합니다.
 * 따라서 h파일 포함하였습니다.
 */
#include "GameplayTagContainer.h"
/**
 * @author 전유진
 * @brief GameplayTags를 Enum에 모아뒀습니다. 오타방지를 위해서입니다.
 */

/**
* @brief GameplayTags를 Enum에 모아뒀습니다. 오타방지를 위해서입니다.
*/
UENUM()
enum class EQGameplayTags : uint8
{
    None,
    Conversing,
};

class QUACKTOHELL_API QGameplayTags
{
public:
	QGameplayTags();
	~QGameplayTags();
    /**
     * @brief 열거형 이름에 맞는 태그를 리턴합니다.
     * 
     * @param EQGameplayTags타입의 EnumValue
     * @return FGameplayTag타입의 게임플레이태그
     */
    static const FGameplayTag GetTag(EQGameplayTags EnumValue);

};
