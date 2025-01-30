// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GodFunction.generated.h"

/**
 * 
 */
UCLASS()
class QUACKTOHELL_API UGodFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// JSON 파일에서 데이터 읽기
	UFUNCTION(BlueprintCallable, Category = "God")
	static FString LoadPromptData();

};
