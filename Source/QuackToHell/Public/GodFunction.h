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

    // OpenAI API를 호출하여 NPC 데이터를 생성하고 JSON 파일로 저장
    UFUNCTION(BlueprintCallable, Category = "God")
    static void GenerateNPCPrompts();

private:
    // OpenAI API 호출
    static FString CallOpenAI(const FString& Prompt);

    // JSON 파일 저장
    static bool SavePromptToFile(const FString& FileName, const FString& Content);

};
