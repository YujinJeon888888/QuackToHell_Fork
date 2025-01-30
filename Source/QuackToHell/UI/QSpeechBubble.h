// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "QSpeechBubble.generated.h"

/**
 * @author 전유진
 * @brief 말풍선 Widget Component입니다. 
 */
UCLASS()
class QUACKTOHELL_API UQSpeechBubble : public UWidgetComponent
{
	GENERATED_BODY()
public:
	/**
	 * @brief 말풍선의 글씨를 업데이트합니다.
	 * @param 새로 띄울 텍스트를 인자로 넣습니다.
	 */
	void UpdateText(FString& Text);
};
