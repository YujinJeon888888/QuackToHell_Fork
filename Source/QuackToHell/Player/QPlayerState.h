// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h" // GameplayTagContainer 사용
#include "GameData/QConversationData.h"
#include "GameData/QEvidenceData.h"
#include "QPlayerState.generated.h"


/**
 * @author 전유진
 * @brief player state클래스입니다.
 */

UCLASS()
class QUACKTOHELL_API AQPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AQPlayerState();
	/**
	 * @brief 플레이어의 상태를 관리하는 GameplayTags.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameplayTags")
	FGameplayTagContainer PlayerStateTags;
	/**
	 * @brief 새로운 태그를 추가합니다.
	 * @param NewStateTag
	 */
	void AddStateTag(FGameplayTag NewStateTag);
	/**
	 * @brief 태그를 지웁니다.
	 * @param 지울 태그
	 */
	void RemoveStateTag(FGameplayTag StateTag);
	/**
	 * @brief 태그를 갖고있는지 체크합니다.
	 * @param 비교할 태그
	 */
	bool HasStateTag(FGameplayTag StateTag) const;

private:
	/** @brief 플레이어가 나눈 모든 대화 정보에 접근할 수 있는 자료 구조입니다 */
	UPROPERTY(Replicated)
	FPlayerConversations ConversationRecordInHand;
	/**  @breif 플레이어가 소지한 증거 정보에 접근할 수 있는 자료구조입니다. */
	UPROPERTY(Replicated)
	FPlayerEvidences EvidenceInHand;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FPlayerConversations GetConversationRecordInHand() const
	{
		return ConversationRecordInHand;
	}
	FPlayerEvidences GetEvidenceInHand() const
	{
		return EvidenceInHand;
	}
	
	// 추가된 증거/대화기록의 ID를 반환
	const int32 AddConversationRecord(int32 ListenerID, int32 SpeakerID, FDateTime Timestamp, const FString& Message);
	const int32 AddEvidence(FString EvidenceName, FString EvidenceDescription, FString EvidenceImagePath);
	void RemoveEvidence(int32 EvidenceID);
	void RemoveAllEvidence();
	
	const FConversationRecord* GetRecordWithConvID(int32 ConversationID) const;
	const TArray<FConversationRecord>& GetAllRecord() const;
	const FEvidence* GetEvidenceWithID(int32 EvidenceID) const;
	const FEvidence* GetEvidenceWithName(FString EvidenceName) const;
	const TArray<FEvidence>& GetAllEvidence() const;

	// for testing conversation & evidence system
	void PrintEvidence(int32 EvidenceID, FString EvidenceName) const;
	void PrintAllEvidence() const;
	void PrintConversation(int32 ConversationID) const;
	void PrintAllConversation(int32 SpeakerID) const;

	virtual void BeginPlay() override;
	TSubclassOf<UUserWidget> StartLevelWidget;
	UFUNCTION(BlueprintCallable)
	void TestAddConversation();
	UFUNCTION(BlueprintCallable)
	void TestAddEvidence();
};




