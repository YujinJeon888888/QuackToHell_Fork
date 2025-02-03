// Copyright_Team_AriAri


#include "UI/QNameWidget.h"
#include "QLogCategories.h"
#include "Components/TextBlock.h" // UTextBlock을 위한 헤더 포함

void UQNameWidget::SetNameWidgetText(const FString& Name)
{
	NameTextBox->SetText(FText::FromString(Name));
	//TODO: EXCEPTION_ACCESS_VIOLATION reading address 0x00000000000004c8
	//1. NameTextBox가 초기화안된듯. 10초뒤 실행하도록 바꿔보기 <- 브로드캐스트를 5초뒤해도, 안된다. 접근하는 것 자체가 문제되는듯. 왜인지보자
	//2. 만약 된다면 nametextbox가 초기화된 후 실행되도록 콜백달기
	
}

void UQNameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogLogic, Log, TEXT("NameWidget의 생성자 호출"));
	

}


