// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Game/QGameModeVillage.h"
#include "QConversationData.generated.h"

/**
 * @author 유서현
 * @brief 대화 기록 데이터 저장을 위한 자료구조 및 열거형이 정의되어있는 헤더파일입니다.
 */

UENUM(BlueprintType)
enum class EConversationState : uint8
{
	None UMETA(DisplayName = "None"),
	P2N UMETA(DisplayName = "P2N"),
	N2N	UMETA(DisplayName = "N2N"),
	NMonologue UMETA(DisplayName = "NMonologue"),
};

USTRUCT(BlueprintType)
struct FConversationRecord
{
	GENERATED_BODY()
private:
	UPROPERTY()
	int32 ConversationID;
	
	UPROPERTY()
	int32 ListenerID;

	UPROPERTY()
	int32 SpeakerID;

	UPROPERTY()
	FDateTime Timestamp;

	UPROPERTY()
	FString Message;

public:
	FConversationRecord(int32& ConversationID, int32& SpeakerID, int32& ListenerID, FDateTime& Timestamp, const FString& Message)
		: ConversationID(ConversationID), ListenerID(ListenerID), SpeakerID(SpeakerID), Timestamp(Timestamp), Message(Message){}
	
	int32 GetConversationID() const {return ConversationID;}
	int32 GetListenerID() const {return ListenerID;}
	int32 GetSpeakerID() const {return SpeakerID;}
	FDateTime GetTimestamp() const {return Timestamp;}
	FString GetMessage() const {return Message;}
};

USTRUCT(BlueprintType)
struct FConversationIDWrapper
{
	GENERATED_BODY()
public:
	TArray<int32> IDs;
};

USTRUCT()
struct FPlayerConversations
{
	GENERATED_BODY()
private:
	// key: SpeakerID value: ConversationID array
	UPROPERTY()
	TMap<int32, FConversationIDWrapper> ConversationIDMap;
	// key : ConversationID value : FConversationRecord
	UPROPERTY()
	TMap<int32, FConversationRecord> ConversationMap;
public:
	void AddConversation(const FConversationRecord& ConversationRecord)
	{
		// 저장할 Record의 ConversationID를 키로 검색했을때 이미 value가 존재한다면 return;
		if (ConversationMap.Find(ConversationRecord.GetConversationID()) != nullptr) return;

		// Speaker와 대화한 기록이 있다면, ConversationIDMap에 Speaker 키를 추가한다.
		// 이후 ConversationIDMap에는 SpeakerID를 Key로 하는 ConversationID array에 새로운 Record의 ConversationID를 추가한 뒤,
		// ConversationMap에는 ConversationID를 키로, 새로운 Record를 Value로 추가한다.
		if (ConversationIDMap.Find(ConversationRecord.GetSpeakerID()) == nullptr)
		{
			FConversationIDWrapper ConversationIDWrapper;
			ConversationIDMap.Add(ConversationRecord.GetSpeakerID(), ConversationIDWrapper);
		}
		ConversationIDMap[ConversationRecord.GetSpeakerID()].IDs.Add(ConversationRecord.GetConversationID());
		ConversationMap.Add(ConversationRecord.GetConversationID(), ConversationRecord);
	}

	// ConversationID에 해당하는 ConversationRecord를 포인터 형태로 반환
	const FConversationRecord* GetRecordWithConvID(const int32& ConversationID) const
	{
		if (ConversationMap.Find(ConversationID) == nullptr) return nullptr;

		return &ConversationMap[ConversationID];
	}

	// Speaker와 나눈 모든 ConversationRecord를 array 형태로 반환
	const TArray<const FConversationRecord*> GetRecordsWithSpeakerID(const int32& SpeakerID) const
	{
		TArray<const FConversationRecord*> ReturnArray;
		const FConversationIDWrapper* ConversationIDs = ConversationIDMap.Find(SpeakerID);
		if (ConversationIDs == nullptr) return ReturnArray;
		if (ConversationIDs->IDs.IsEmpty()) return ReturnArray;

		for (int32 ConversationID : ConversationIDs->IDs)
		{
			if (const FConversationRecord* Found = ConversationMap.Find(ConversationID))
			{
				ReturnArray.Add(Found);
			}
		}
		return ReturnArray;
	}
};

UCLASS()
class QUACKTOHELL_API UQConversationData : public UObject
{
	GENERATED_BODY()
};