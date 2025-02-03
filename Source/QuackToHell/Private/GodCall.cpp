// Copyright_Team_AriAri


#include "GodCall.h"
#include "GodFunction.h"

// Sets default values for this component's properties
UGodCall::UGodCall()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGodCall::BeginPlay()
{
	Super::BeginPlay();

}

void UGodCall::StartGodProcess()
{
    UE_LOG(LogTemp, Log, TEXT("God process started. Generating NPC Prompts..."));

    // 새롭게 업데이트된 `GenerateNPCPrompts()` 호출
    UGodFunction::GenerateNPCPrompts();

    UE_LOG(LogTemp, Log, TEXT("God process finished NPC generation."));
}

void UGodCall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

