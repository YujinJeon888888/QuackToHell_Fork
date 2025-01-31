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
    FString PromptData = UGodFunction::LoadPromptData();
    if (!PromptData.Contains(TEXT("Error")))
    {
        UGodFunction::GenerateNPCDataFromOpenAI();
        UE_LOG(LogTemp, Log, TEXT("God process started. Generating NPC data..."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to start God process: Could not load PromptToGod.json"));
    }
}

// Called every frame
void UGodCall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

