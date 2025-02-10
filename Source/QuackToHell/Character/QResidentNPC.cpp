// Copyright_Team_AriAri


#include "Character/QResidentNPC.h"
#include "ResidentComponent.h"

AQResidentNPC::AQResidentNPC(const FObjectInitializer& ObjectInitializer)
	: Super(
		ObjectInitializer.SetDefaultSubobjectClass<UResidentComponent>(TEXT("NPCComponent"))
	)
{

}

void AQResidentNPC::BeginPlay()
{
	Super::BeginPlay();
}


