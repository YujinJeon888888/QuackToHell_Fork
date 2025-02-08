// Copyright_Team_AriAri

#include "GodCall.h"
#include "GodFunction.h"

// Sets default values for this component's properties
UGodCall::UGodCall()
{
    PrimaryComponentTick.bCanEverTick = false; // Tick 비활성화
}

void UGodCall::PostInitProperties()
{
    Super::PostInitProperties();

    FString PromptFolder = FPaths::ProjectSavedDir() + TEXT("Prompt/");
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    if (!PlatformFile.DirectoryExists(*PromptFolder))
    {
        if (PlatformFile.CreateDirectoryTree(*PromptFolder))
        {
            UE_LOG(LogTemp, Log, TEXT("✅ Created Prompt directory: %s"), *PromptFolder);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ Failed to create Prompt directory: %s"), *PromptFolder);
        }
    }
}

void UGodCall::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("✅ GodCall::BeginPlay() - Deleting old prompts and preparing for game start..."));

    UWorld* World = GetOwner() ? GetOwner()->GetWorld() : nullptr;
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Failed to get World context from GetOwner()->GetWorld()!"));
        return;
    }

    // 기존 Prompt 파일 삭제
    UGodFunction::DeleteOldPromptFiles();

    // ✅ StartGodProcess()를 통해 defendant 생성 및 게임 진행
    UE_LOG(LogTemp, Log, TEXT("✅ GodCall::BeginPlay() 완료. StartGodProcess()를 통해 피고인 생성 진행."));
}


bool UGodCall::StartGodProcess()
{
    UE_LOG(LogTemp, Log, TEXT("✅ StartGodProcess() called! Generating Defendant Prompt first..."));

    UWorld* World = GetOwner() ? GetOwner()->GetWorld() : nullptr;
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Failed to get World context from GetOwner()->GetWorld!"));
        return false;
    }

    // ✅ 이미 피고인 데이터가 존재하면 다시 생성하지 않음
    FString DefendantFilePath = FPaths::ProjectSavedDir() + TEXT("Prompt/PromptToDefendant.json");
    if (FPaths::FileExists(DefendantFilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ PromptToDefendant.json 이미 존재하므로 다시 생성하지 않음."));
        UGodFunction::GenerateJuryNPC(World, 1);
        return true;
    }

    // 🔥 피고인 데이터 생성
    UGodFunction::GenerateDefendantPrompt(World, [World]()
        {
            UE_LOG(LogTemp, Log, TEXT("✅ PromptToDefendant.json 생성 완료! 배심원 생성 시작."));
            UGodFunction::GenerateJuryNPC(World, 1);
        });

    return true;
}


void UGodCall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

