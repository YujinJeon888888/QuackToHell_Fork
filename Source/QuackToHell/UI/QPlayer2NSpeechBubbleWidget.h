// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "QPlayer2NSpeechBubbleWidget.generated.h"

/**
 * @author 전유진
 * @brief 공백 말풍선에 대한 widget component입니다. 상대방 플레이어가 npc와 대화 시 해당 말풍선이 대화 대상들의 머리 위에 뜹니다.
 */
UCLASS()
class QUACKTOHELL_API UQPlayer2NSpeechBubbleWidget : public UWidgetComponent
{
	GENERATED_BODY()
public:
	/**
	 * @brief 말풍선 UI 컴포넌트를 킵니다.
	 *
	 */
	void TurnOnSpeechBubble();
	/**
	 * @brief 말풍선 UI 컴포넌트를 끕니다.
	 *
	 */
	void TurnOffSpeechBubble();
};
