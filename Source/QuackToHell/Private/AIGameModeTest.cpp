// Copyright_Team_AriAri

#include "AIGameModeTest.h"
#include "GodCall.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

void AAIGameModeTest::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("✅ AIGameModeTest BeginPlay() started!"));

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ AIGameModeTest::BeginPlay() - Failed to get valid World!"));
        return;
    }

    // 🔥 기존 GodActor가 있어도 실행되도록 수정
    AActor* GodActor = nullptr;
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(World, TEXT("GodActor"), FoundActors);

    if (FoundActors.Num() > 0)
    {
        GodActor = FoundActors[0];
        UE_LOG(LogTemp, Warning, TEXT("⚠️ Existing GodActor found, reusing it."));
    }
    else
    {
        GodActor = World->SpawnActor<AActor>(AActor::StaticClass());
        if (!GodActor)
        {
            UE_LOG(LogTemp, Error, TEXT("❌ Failed to create God Actor!"));
            return;
        }
        GodActor->Tags.Add(TEXT("GodActor"));
        UE_LOG(LogTemp, Log, TEXT("✅ New GodActor created!"));
    }

    // 🔥 UGodCall 실행 (기존에 있어도 실행하도록 변경)
    UGodCall* GodCall = GodActor->FindComponentByClass<UGodCall>();
    if (!GodCall)
    {
        GodCall = NewObject<UGodCall>(GodActor);
        if (!GodCall)
        {
            UE_LOG(LogTemp, Error, TEXT("❌ Failed to create GodCall component!"));
            return;
        }
        GodCall->RegisterComponent();
        UE_LOG(LogTemp, Log, TEXT("✅ GodCall component created and registered!"));
    }

    GodCall->StartGodProcess();
}

