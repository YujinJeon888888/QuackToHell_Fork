// Copyright_Team_AriAri


#include "Player/QPlayerState.h"
#include "QLogCategories.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

void AQPlayerState::AddStateTag(FGameplayTag NewStateTag)
{
	PlayerStateTags.AddTag(NewStateTag);
	UE_LOG(LogLogic, Log, TEXT("플레이어 상태 태그 추가: %s"), *NewStateTag.ToString());
}

void AQPlayerState::RemoveStateTag(FGameplayTag StateTag)
{
	PlayerStateTags.RemoveTag(StateTag);
	UE_LOG(LogLogic, Log, TEXT("플레이어 상태 태그 제거: %s"), *StateTag.ToString());
}

bool AQPlayerState::HasStateTag(FGameplayTag StateTag) const
{
	return PlayerStateTags.HasTagExact(StateTag);
}

void AQPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AQPlayerState, ConversationRecords);
	DOREPLIFETIME(AQPlayerState, EvidenceInHand);
}

const int32 AQPlayerState::AddConversationRecord(int32 ListenerID, int32 SpeakerID, FDateTime Timestamp, const FString& Message)
{
	AQGameModeVillage* GameModeVillage = Cast<AQGameModeVillage>(GetWorld()->GetAuthGameMode());
	int32 ConversationID = GameModeVillage->ConversationIDCount++;
	FConversationRecord NewConversationRecord(ConversationID, SpeakerID, ListenerID, Timestamp, Message);
	ConversationRecords.AddConversation(NewConversationRecord);

	return ConversationID;
}

const int32 AQPlayerState::AddEvidence(FString EvidenceName, FString EvidenceDescription, FString EvidenceImagePath)
{
	AQGameModeVillage* GameModeVillage = Cast<AQGameModeVillage>(GetWorld()->GetAuthGameMode());
	int32 EvidenceID = GameModeVillage->EvidenceIDCount++;
	FEvidence NewEvidence(EvidenceID, EvidenceName, EvidenceDescription, EvidenceImagePath);
	EvidenceInHand.AddEvidence(NewEvidence);

	return EvidenceID;
}

void AQPlayerState::RemoveEvidence(int32 EvidenceID)
{
	EvidenceInHand.RemoveEvidence(EvidenceID);
}

void AQPlayerState::RemoveAllEvidence()
{
	EvidenceInHand.RemoveAllEvidence();
}

const FConversationRecord* AQPlayerState::GetRecordWithConvID(int32 ConversationID) const
{
	return ConversationRecords.GetRecordWithConvID(ConversationID);
}

const TArray<const FConversationRecord*> AQPlayerState::GetRecordWithSpeaker(int32 SpeakerID) const
{
	return ConversationRecords.GetRecordsWithSpeakerID(SpeakerID);
}

const FEvidence* AQPlayerState::GetEvidenceWithID(int32 EvidenceID) const
{
	return EvidenceInHand.GetEvidenceWithID(EvidenceID);
}

const FEvidence* AQPlayerState::GetEvidenceWithName(FString EvidenceName) const
{
	return EvidenceInHand.GetEvidenceWithName(EvidenceName);
}

EvidenceList AQPlayerState::GetAllEvidence() const
{
	return EvidenceInHand.GetAllEvidence();
}



// -------------------------------------------------------------------------- //

void AQPlayerState::PrintEvidence(int32 EvidenceID, FString EvidenceName) const
{
	const FEvidence* EvWithName = EvidenceInHand.GetEvidenceWithName(EvidenceName);
	const FEvidence* EvWithID = EvidenceInHand.GetEvidenceWithID(EvidenceID);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
				FString::Printf(TEXT("Found Evidence with ID - Evidence Name: %s, EvidenceID: %d"), *EvWithID->GetName(), EvWithID->GetID()));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
					FString::Printf(TEXT("Found Evidence with Name - Evidence Name: %s, EvidenceID: %d"), *EvWithName->GetName(), EvWithName->GetID()));
	}
}

void AQPlayerState::PrintAllEvidence() const
{
	EvidenceList PlayerEvidences = EvidenceInHand.GetAllEvidence();
	if (GEngine)
	{
		for (const auto& Evidence : PlayerEvidences)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
				FString::Printf(TEXT("Found All Evidence - Evidence Name: %s, EvidenceID: %d"), *Evidence->GetName(), Evidence->GetID()));
		}
	}
}

void AQPlayerState::PrintConversation(int32 ConversationID) const
{
	const FConversationRecord* Record = ConversationRecords.GetRecordWithConvID(ConversationID);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
				FString::Printf(TEXT("Found Conversation with ID - Speaker: %d, Listener: %d at %s \n Conversation message: %s"), Record->GetSpeakerID(), Record->GetListenerID(), *Record->GetTimestamp().ToString(),*Record->GetMessage()));
	}
}

void AQPlayerState::PrintAllConversation(int32 SpeakerID) const
{
	const TArray<const FConversationRecord*> Records = ConversationRecords.GetRecordsWithSpeakerID(SpeakerID);
	if (GEngine)
	{
		for (const auto& Record : Records)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
			FString::Printf(TEXT("Found Conversation with Speaker - Speaker: %d, Listener: %d at %s\n Conversation message: %s"), Record->GetSpeakerID(), Record->GetListenerID(), *Record->GetTimestamp().ToString(),*Record->GetMessage()));
		}
	}
}

void AQPlayerState::TestAddConversation()
{
	FDateTime Timestamp = FDateTime::Now();
	int32 ConversationID = AddConversationRecord(1000, 1001, Timestamp, FString::Printf(TEXT("TestAddConversation s: %d  l:%d"), 1000, 1001));

	PrintConversation(ConversationID);
	PrintAllConversation(1001);
}

void AQPlayerState::TestAddEvidence()
{
	int32 EvidenceID1 = AddEvidence(TEXT("Evidence1"), TEXT("Evidence1 description"), TEXT("Evidence1 Image Path"));
	int32 EvidenceID2 = AddEvidence(TEXT("Evidence2"), TEXT("Evidence2 description"), TEXT("Evidence2 Image Path"));
	int32 EvidenceID3 = AddEvidence(TEXT("Evidence3"), TEXT("Evidence3 description"), TEXT("Evidence3 Image Path"));

	PrintEvidence(EvidenceID1, "Evidence1");
	PrintEvidence(EvidenceID2, "Evidence2");
	PrintAllEvidence();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("Remove Evidence1"));
	}

	RemoveEvidence(EvidenceID1);
	PrintAllEvidence();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("Remove all Evidence"));
	}
	
	RemoveAllEvidence();
	PrintAllEvidence();
}


