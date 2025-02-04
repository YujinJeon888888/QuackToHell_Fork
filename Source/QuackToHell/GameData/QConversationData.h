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
	FConversationRecord(){}
	
	FConversationRecord(int32& ConversationID, int32& SpeakerID, int32& ListenerID, FDateTime& Timestamp, const FString& Message)
		: ConversationID(ConversationID), ListenerID(ListenerID), SpeakerID(SpeakerID), Timestamp(Timestamp), Message(Message){}

	bool operator==(const FConversationRecord& Other) const
	{
		return ConversationID == Other.ConversationID;
	}
	
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
private:UPROPERTY()
	TArray<FConversationRecord> ConversationList;
public:
	void AddConversation(const FConversationRecord& ConversationRecord)
	{
		if (ConversationList.Contains(ConversationRecord)) return;

		ConversationList.Add(ConversationRecord);
	}

	// ConversationID에 해당하는 ConversationRecord를 반환
	const FConversationRecord* GetRecordWithConvID(const int32& ConversationID) const
	{
		for (const auto& ConversationRecord : ConversationList)
		{
			if (ConversationRecord.GetConversationID() == ConversationID)
			{
				return &ConversationRecord;
			}
		}
		return nullptr;
	}

	const TArray<FConversationRecord>& GetAllRecord() const
	{
		return ConversationList;
	}
};

UCLASS()
class QUACKTOHELL_API UQConversationData : public UObject
{
	GENERATED_BODY()
};