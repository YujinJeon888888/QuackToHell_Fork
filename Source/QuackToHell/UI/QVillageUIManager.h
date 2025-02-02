// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "QVillageUIManager.generated.h"
UENUM()
/** @brief 마을 내 존재해야하는 UI목록들 */
enum class EVillageUIType :uint8 {
	P2N= 0,

};
UCLASS()
/**
 * @author 전유진
 * @brief 마을 맵 내에서만 존재하는 마을 UI manager
 */
class QUACKTOHELL_API AQVillageUIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQVillageUIManager();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	/**
	 * @brief UI를 켭니다.
	 * 
	 * @param UIType
	 */
	void TurnOnUI(EVillageUIType UIType);
	/**
	 * @brief UI를 끕니다.
	 *
	 * @param UIType
	 */
	void TurnOffUI(EVillageUIType UIType);
	/**
	 * @brief 싱글톤 인스턴스 반환.
	 * @return 싱글톤 인스턴스
	 */
	static TObjectPtr<AQVillageUIManager> GetInstance(TObjectPtr<UWorld> World);
	/**
	 * @brief 화면에 보이는 위젯들을 리턴합니다..
	 * 
	 * @return 화면에 보이는 위젯들의 map 
	 */
	TMap<EVillageUIType, TObjectPtr<UUserWidget>> GetActiveWidgets() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/** @brief 싱글톤 인스턴스 */
	static TObjectPtr<AQVillageUIManager> Instance;
	/** @brief 마을 맵 언로드될 때 UI들 파괴 */
	void DestroyAllUI();
	/** @brief  UI타입과 위젯클래스 매핑*/
	TMap<EVillageUIType, TSubclassOf<UUserWidget>> UIWidgetClasses;
	/** @brief 활성화 된 위젯 관리 */
	TMap<EVillageUIType, TObjectPtr<UUserWidget>> ActiveWidgets;
	
	/** 맵 로드 시 실행 */
	void OnMapLoad();
	/** 맵 언로드 시 실행 */
	void OnMapUnLoad();
};




