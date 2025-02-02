// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QP2NWidget.generated.h"


/**
 * @author 전유진
 * @brief P2N 대화 UI입니다.
 */
UCLASS()
class QUACKTOHELL_API UQP2NWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * @brief 대화상자의 글씨를 업데이트합니다.
	 * @param 새로 띄울 텍스트를 인자로 넣습니다.
	 */
	void UpdatePlayerText(FString& Text);
	/**
	 * @brief 대화상자의 글씨를 업데이트합니다.
	 * @param 새로 띄울 텍스트를 인자로 넣습니다.
	 */
	void UpdateNPCText(FString& Text);
private:
	/**
	 * @brie PlayerText 컴포넌트 바인드
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PlayerText;
	/**
	 * @brie NPCText 컴포넌트 바인드
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> NPCText;
};
