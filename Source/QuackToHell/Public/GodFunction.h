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

	// OpenAI API를 호출하여 새로운 NPC 데이터를 생성하고, JSON 응답을 받아 Content\Prompts 폴더에 저장
	UFUNCTION(BlueprintCallable, Category = "God")
	static void GenerateNPCDataFromOpenAI();

};
