// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QConversationData.generated.h"

/**
 * @author 유서현
 * @brief 대화 기록 데이터 저장을 위한 자료구조 및 열거형이 정의되어있는 헤더파일입니다.
 */

/**
 * @brief 대화 유형을 나타내는 Enum
 */
UENUM(BlueprintType)
enum class EConversationType : uint8
{
	None UMETA(DisplayName = "None"),
	P2N UMETA(DisplayName = "P2N"),  // 플레이어 ↔ NPC 대화
	N2N UMETA(DisplayName = "N2N"),    // NPC ↔ NPC 대화
	NMonologue UMETA(DisplayName = "NMonologue") // NPC 혼잣말
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
	
	FConversationRecord(int32 ConversationID, int32 SpeakerID, int32 ListenerID, FDateTime Timestamp, const FString& Message)
		: ConversationID(ConversationID), ListenerID(ListenerID), SpeakerID(SpeakerID), Timestamp(Timestamp), Message(Message){}

	bool operator==(const FConversationRecord& Other) const
	{
		return ConversationID == Other.ConversationID;
	}

	FConversationRecord& operator=(const FConversationRecord& Other)
	{
		if (this != &Other)
		{
			ConversationID = Other.ConversationID;
			ListenerID = Other.ListenerID;
			SpeakerID = Other.SpeakerID;
			Timestamp = Other.Timestamp;
			Message = Other.Message;
		}
		return *this;
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
struct FConversationList
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
	const TArray<FConversationRecord>& GetConversationList() const
	{
		return ConversationList;
	}

	// ConversationID에 해당하는 ConversationRecord를 반환
	const FConversationRecord* GetRecordWithConvID(int32 ConversationID) const
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

	// Player가 소유하고 있는 대화기록에 대한 정보만 반환
	const TArray<FConversationRecord> GetRecordWithID(int32 ID) const
	{
		return ConversationList.FilterByPredicate([ID](const FConversationRecord& Record)
		{
			return (Record.GetSpeakerID() == ID || Record.GetListenerID() == ID);
		});
	}
};

UCLASS()
class QUACKTOHELL_API UQConversationData : public UObject
{
	GENERATED_BODY()
};