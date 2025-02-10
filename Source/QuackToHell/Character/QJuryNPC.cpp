// Copyright_Team_AriAri


#include "Character/QJuryNPC.h"
#include "JuryComponent.h"

AQJuryNPC::AQJuryNPC(const FObjectInitializer& ObjectInitializer)
	: Super(
		ObjectInitializer.SetDefaultSubobjectClass<UJuryComponent>(TEXT("NPCComponent"))
	)
{

}

void AQJuryNPC::BeginPlay()
{
	Super::BeginPlay();
}


