// Copyright_Team_AriAri


#include "UI/QVillageUIManager.h"
#include "UI/QP2NWidget.h"
#include "Kismet/GameplayStatics.h"

#include "QLogCategories.h"

TObjectPtr<AQVillageUIManager> AQVillageUIManager::Instance = nullptr;

// Sets default values
AQVillageUIManager::AQVillageUIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//UI타입과 UI클래스 매핑
	//TODO: 마을 내 유아이들 전부 추가해주기
	static ConstructorHelpers::FClassFinder<UQP2NWidget> P2NWidgetAsset(TEXT("WidgetBlueprint'/Game/Blueprints/UI/WBP_QP2NWidget.WBP_QP2NWidget_C'"));

	// TSubclassOf 템플릿 클래스 객체에 블루프린트 클래스를 넣어준다
	if (P2NWidgetAsset.Succeeded())
	{
		UIWidgetClasses.Add(EVillageUIType::P2N, P2NWidgetAsset.Class);
	}
	

}

void AQVillageUIManager::TurnOffUI(EVillageUIType UIType)
{
	if (ActiveWidgets.Contains(UIType)) {
		ActiveWidgets[UIType]->SetVisibility(ESlateVisibility::Hidden);
		//TODO: 활성위젯에서삭제
		ActiveWidgets.Remove(UIType);
	}
}

TObjectPtr<AQVillageUIManager> AQVillageUIManager::GetInstance(TObjectPtr<UWorld> World)
{
	if (!Instance) {
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(World, AQVillageUIManager::StaticClass(),FoundActors);
		if (FoundActors.Num() > 0) {
			Instance = Cast<AQVillageUIManager>(FoundActors[0]);
		}
	}
	return Instance;
}

TMap<EVillageUIType, TObjectPtr<UUserWidget>> AQVillageUIManager::GetActiveWidgets() const
{
	return ActiveWidgets;
}

// Called when the game starts or when spawned
void AQVillageUIManager::BeginPlay()
{
	Super::BeginPlay();
	if (!Instance) {
		Instance = this;
	}
	//마을 맵에서만 VillageUIManager가 동작하도록
	OnMapLoad();
	
}

void AQVillageUIManager::DestroyAllUI()
{
	for (auto& WidgetPair : ActiveWidgets) {
		if (WidgetPair.Value) {
			// 화면에서 제거
			WidgetPair.Value->RemoveFromParent();  
			// 객체 메모리 해제
			WidgetPair.Value->ConditionalBeginDestroy();  
		}
	}
	ActiveWidgets.Empty();
}

void AQVillageUIManager::OnMapLoad()
{
	FString CurrentMap = UGameplayStatics::GetCurrentLevelName(this);

	//TODO: 마을맵으로바꾸기  / 마을맵이면 기본으로 활성화 할 것들
	//마을맵이면 기본으로 활성화 할 것들
	if (CurrentMap == "ClinetTestMap") {
		//TODO: 기본 마을 UI들 띄운다 (초기화작업)
		TurnOnUI(EVillageUIType::P2N);
	}
	
}

void AQVillageUIManager::OnMapUnLoad()
{
	DestroyAllUI();
	Instance = nullptr;
	Destroy();
}

// Called every frame
void AQVillageUIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQVillageUIManager::TurnOnUI(EVillageUIType UIType)
{
	//위젯 이미 만들어져있으면 visible 전환
	if (ActiveWidgets.Contains(UIType)) {
		ActiveWidgets[UIType]->SetVisibility(ESlateVisibility::Visible);
		return;
	}

	//마을 UI로 타입을 넣어두지않았으면
	if (!UIWidgetClasses[UIType]) {
		UE_LOG(LogLogic, Warning, TEXT("QVillageUIManager: UIClassWidgetClasses에 UI타입에 맞는 값을 생성하세요"));
		return;
	}

	TObjectPtr<UWorld> World = GetWorld();
	if (!World) return;
	//위젯 새로 만들기
	TObjectPtr<UUserWidget> NewWidget = CreateWidget<UUserWidget>(World, UIWidgetClasses[UIType]);
	if (NewWidget) {
		NewWidget->AddToViewport();
		NewWidget->SetVisibility(ESlateVisibility::Visible);  
		ActiveWidgets.Add(UIType, NewWidget);
	}
}



