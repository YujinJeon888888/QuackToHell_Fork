// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QItemData.generated.h"
/**
 * @brief 아이템 유형입니다.
 */
UENUM(BlueprintType)//블루프린트에서 해당 열거형(enum)을 사용할 수 있도록 노출
enum class EItemType : uint8
{
	Evidence = 0,
	CD,
};

/**
 * @author 전유진
 * @brief 아이템 데이터에 대한 클래스입니다.
 */
UCLASS()
class QUACKTOHELL_API UQItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;
};
