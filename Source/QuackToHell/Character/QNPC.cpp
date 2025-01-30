// Copyright_Team_AriAri


#include "Character/QNPC.h"
#include "Components/CapsuleComponent.h"
AQNPC::AQNPC()
{	/*위치값 세팅*/
	this->GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
	this->GetMesh()->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	/*캡슐 콜라이더 세팅*/
	this->GetCapsuleComponent()->InitCapsuleSize(50.0f, 60.0f);
}

bool AQNPC::ServerRPCCanCanStartConversN2N_Implementation(TObjectPtr<AQNPC> NPC)
{
	// NPC null체크
	if (NPC == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NPC is null"));
		return false;
	}
	return true;
}

