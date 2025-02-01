// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h" // UUserWidget 포함 필요
#include "QNameWidget.generated.h"


/**
 * @author 전유진
 * @brief 이름 UI 클래스
 */

UCLASS()
class QUACKTOHELL_API UQNameWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	/**
	 * @brief 이름 UI의 text를 업데이트합니다.
	 * 
	 */
	void SetNameWidgetText(const FString& Name);

	virtual void NativeConstruct() override;

protected:
	/**
	 * @brief 블루프린트의 NameTextBox와 연결.
	 */
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<class UTextBlock> NameTextBox;


};
