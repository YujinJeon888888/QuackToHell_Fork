// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "QSpeechBubbleWidgetComponent.generated.h"

/**
 * @author 전유진
 * @brief 캐릭터의 머리 위에 뜨는 말풍선 UI 컴포넌트입니다. .
 */
UCLASS()
class QUACKTOHELL_API UQSpeechBubbleWidgetComponent : public UWidgetComponent
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
	/**
	 * @brief 말풍선의 글씨를 업데이트합니다.
	 * @param 새로 띄울 텍스트를 인자로 넣습니다.
	 */
	void UpdateText(FString& Text);
};
