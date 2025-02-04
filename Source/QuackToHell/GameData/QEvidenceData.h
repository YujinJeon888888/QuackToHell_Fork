// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "QEvidenceData.generated.h"

/**
 * @author 유서현
 * @brief 증거 데이터 저장을 위한 자료구조 및 열거형이 정의되어있는 헤더파일입니다.
 */
typedef const TArray<const FEvidence*> EvidenceList;

USTRUCT(BlueprintType)
struct FEvidence
{
	GENERATED_BODY()

private:
	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString Description;

	UPROPERTY()
	FString ImagePath;

	FEvidence(int32& EvidenceID, FString& EvidenceName, FString& EvidenceDescription, FString& EvidenceImagePath)
		: ID(EvidenceID), Name(EvidenceName), Description(EvidenceDescription), ImagePath(EvidenceImagePath){}

	FEvidence() : ID(-1), Name(""), Description(""), ImagePath(""){}

public:
	int32 GetID() const  { return ID; }
	FString GetName() const { return Name; }
	FString GetDescription() const { return Description; }
	FString GetImagePath() const { return ImagePath; }
};

USTRUCT(BlueprintType)
struct FPlayerEvidences
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TMap<int32, FEvidence> Evidences;
	TMap<FString, int32> EvidenceIDs;

public:
	// 증거Map에 새로운 증거 추가
	void AddEvidence(const FEvidence& NewEvidence)
	{
		Evidences.Add(NewEvidence.GetID(), NewEvidence);
		EvidenceIDs.Add(NewEvidence.GetName(), NewEvidence.GetID());
	}

	void RemoveEvidence(const int32& EvidenceID)
	{
		if (!Evidences.Contains(EvidenceID)) return;

		FEvidence* Target = Evidences.Find(EvidenceID);
		EvidenceIDs.Remove(Target->GetName());
		Evidences.Remove(EvidenceID);
	}

	void RemoveAllEvidence()
	{
		if (Evidences.IsEmpty()) return;

		Evidences.Empty();
	}

	// ID로 증거 검색 (const 버전)
	const FEvidence* GetEvidenceWithID(const int32& EvidenceID) const
	{
		if (const FEvidence* FoundEvidence = Evidences.Find(EvidenceID))
		{
			return FoundEvidence;
		}
		UE_LOG(LogTemp, Warning, TEXT("GetEvidenceWithID: Evidence with ID %d not found"), EvidenceID);
		return nullptr;
	}

	// 이름으로 증거 검색 (const 버전)
	const FEvidence* GetEvidenceWithName(const FString& EvidenceName) const
	{
		const int32 FoundID = *EvidenceIDs.Find(EvidenceName);
		if (const FEvidence* FoundEvidence = Evidences.Find(FoundID))
		{
			return FoundEvidence;
		}
		UE_LOG(LogTemp, Warning, TEXT("GetEvidenceWithName: Evidence with name '%s' not found"), *EvidenceName);
		return nullptr;
	}

	EvidenceList GetAllEvidence() const
	{
		TArray<const FEvidence*> FoundEvidences;
		for (const auto& Evidence : Evidences)
		{
			const FEvidence* E = &Evidence.Value;
			FoundEvidences.Add(E);
		}
		return FoundEvidences;
	}
};

UCLASS()
class QUACKTOHELL_API UQEvidenceData : public UObject
{
	GENERATED_BODY()
	
};
