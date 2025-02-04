// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "QEvidenceData.generated.h"

/**
 * @author 유서현
 * @brief 증거 데이터 저장을 위한 자료구조 및 열거형이 정의되어있는 헤더파일입니다.
 */
typedef const TArray<const FEvidence*> FoundEvidenceList;

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

public:
	FEvidence(int32& EvidenceID, FString& EvidenceName, FString& EvidenceDescription, FString& EvidenceImagePath)
		: ID(EvidenceID), Name(EvidenceName), Description(EvidenceDescription), ImagePath(EvidenceImagePath){}

	FEvidence() : ID(-1), Name(""), Description(""), ImagePath(""){}
	
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
	TArray<FEvidence> EvidenceList;

public:
	// 증거Map에 새로운 증거 추가
	void AddEvidence(FEvidence& NewEvidence)
	{
		EvidenceList.Add(NewEvidence);
	}

	void RemoveEvidence(const int32& EvidenceID)
	{
		for (int32 i = 0; i < EvidenceList.Num(); ++i)
		{
			if (EvidenceList[i].GetID() == EvidenceID)
			{
				EvidenceList.RemoveAt(i);
				break; // 첫 번째로 찾은 요소만 삭제 (중복된 ID가 없을 경우)
			}
		}
	}

	void RemoveAllEvidence()
	{
		if (EvidenceList.IsEmpty()) return;

		EvidenceList.Empty();
	}

	// ID로 증거 검색 
	const FEvidence* GetEvidenceWithID(const int32& EvidenceID) const
	{
		for (const auto& Target : EvidenceList)
		{
			if (Target.GetID() == EvidenceID) return &Target;
		}
		return nullptr;
	}

	// 이름으로 증거 검색
	const FEvidence* GetEvidenceWithName(const FString& EvidenceName) const
	{
		for (const auto& Target : EvidenceList)
		{
			if (Target.GetName() == EvidenceName) return &Target;
		}
		return nullptr;
	}

	const TArray<FEvidence>& GetAllEvidence() const
	{
		return EvidenceList;
	}
};

UCLASS()
class QUACKTOHELL_API UQEvidenceData : public UObject
{
	GENERATED_BODY()
	
};
