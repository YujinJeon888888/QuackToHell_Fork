// Copyright_Team_AriAri

#include "AIGameModeTest.h"
#include "GodCall.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

void AAIGameModeTest::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("AAIGameModeTest::BeginPlay() 실행됨"));

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ BeginPlay - World is nullptr!"));
        return;
    }

    // 기존 GodActor 찾기
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(World, TEXT("GodActor"), FoundActors);

    if (FoundActors.Num() == 0 || !FoundActors[0])
    {
        UE_LOG(LogTemp, Error, TEXT("❌ BeginPlay - GodActor를 찾을 수 없음!"));
        return;
    }

    AActor* GodActor = FoundActors[0];
    UGodCall* GodCall = GodActor->FindComponentByClass<UGodCall>();

    if (!GodCall)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ BeginPlay - GodCall을 찾을 수 없음!"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("✅ BeginPlay - GodCall StartGodProcess 호출!"));
    GodCall->StartGodProcess();
}
