// Copyright_Team_AriAri


#include "GodCall.h"
#include "GodFunction.h"

// Sets default values for this component's properties
UGodCall::UGodCall()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGodCall::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGodCall::StartGodProcess()
{
	FString PromptData = UGodFunction::LoadPromptData();
	UE_LOG(LogTemp, Log, TEXT("God Generated Data: %s"), *PromptData);
}

// Called every frame
void UGodCall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

