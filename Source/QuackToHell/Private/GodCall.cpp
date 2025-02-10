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
    UE_LOG(LogTemp, Log, TEXT("UGodCall::PostInitProperties() 실행됨!"));

    FString PromptFolder = FPaths::ProjectSavedDir() + TEXT("Prompt/");
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    if (!PlatformFile.DirectoryExists(*PromptFolder))
    {
        if (PlatformFile.CreateDirectoryTree(*PromptFolder))
        {
            UE_LOG(LogTemp, Log, TEXT("Created Prompt directory: %s"), *PromptFolder);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create Prompt directory: %s"), *PromptFolder);
            return;
        }
    }

    FString DefendantFilePath = PromptFolder + TEXT("PromptToDefendant.json");
    if (FPaths::FileExists(DefendantFilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("기존 Prompt 파일이 존재하므로 삭제하지 않음."));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Prompt 파일이 없으므로 즉시 생성 시작!"));
    StartGodProcess();
}

bool UGodCall::bShouldStopPromptGeneration = false;
bool UGodCall::bIsPromptGenerating = false;

void UGodCall::BeginPlay()
{
    Super::BeginPlay();


    UE_LOG(LogTemp, Log, TEXT("GodCall::BeginPlay() 실행됨. bShouldStopPromptGeneration = %d"), bShouldStopPromptGeneration);

    FString DefendantFilePath = FPaths::ProjectSavedDir() + TEXT("Prompt/PromptToDefendant.json");

    if (!FPaths::FileExists(DefendantFilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("프롬프트 파일이 존재하지 않음. 다시 생성 시작!"));
        StartGodProcess();
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("기존 프롬프트가 존재하므로 재생성하지 않음."));
    }
}

void UGodCall::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    UE_LOG(LogTemp, Log, TEXT("게임 종료됨! 기존 Prompt 파일 삭제 시작"));

    // 기존 Prompt 파일 삭제
    UGodFunction::DeleteOldPromptFiles();

    // Prompt 파일 삭제 후 생성 가능하도록 설정
    bShouldStopPromptGeneration = false;

    UE_LOG(LogTemp, Log, TEXT("기존 Prompt 파일 삭제 완료! 다음 실행에서 프롬프트가 재생성됨. bShouldStopPromptGeneration = %d"), bShouldStopPromptGeneration);
}

bool UGodCall::StartGodProcess()
{
    UE_LOG(LogTemp, Log, TEXT("StartGodProcess 실행됨."));

    if (bIsPromptGenerating)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartGodProcess()가 이미 실행 중이므로 중단!"));
        return false;
    }

    bIsPromptGenerating = true;

    UWorld* World = GetOwner() ? GetOwner()->GetWorld() : nullptr;
    if (GetOwner())
    {
        UE_LOG(LogTemp, Log, TEXT("GodCall's owner is %s"), *GetOwner()->GetName());
    }
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("StartGodProcess() 실패! World를 가져올 수 없습니다."));
        bIsPromptGenerating = false;
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("StartGodProcess - World 정상 인식."));

    FString DefendantFilePath = FPaths::ProjectSavedDir() + TEXT("Prompt/PromptToDefendant.json");

    if (FPaths::FileExists(DefendantFilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("PromptToDefendant.json 이미 존재하므로 다시 생성하지 않음."));
        bIsPromptGenerating = false;
        UGodFunction::GenerateJuryNPC(World, 1);
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("피고인 데이터 생성 시작."));

    UGodFunction::GenerateDefendantPrompt(World, [World]()
        {
            if (!World)
            {
                UE_LOG(LogTemp, Error, TEXT("StartGodProcess Callback - World is nullptr!"));
                return;
            }

            UE_LOG(LogTemp, Log, TEXT("PromptToDefendant.json 생성 완료! 배심원 생성 시작."));
            bIsPromptGenerating = false;

            FString DefendantFilePath = FPaths::ProjectSavedDir() + TEXT("Prompt/PromptToDefendant.json");
            if (FPaths::FileExists(DefendantFilePath))
            {
                UE_LOG(LogTemp, Log, TEXT("피고인 프롬프트 파일 확인됨. 배심원 생성 시작."));
                UGodFunction::GenerateJuryNPC(World, 1);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("PromptToDefendant.json 생성 확인 실패! 배심원 생성 중단."));
            }
        });

    return true;
}

void UGodCall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

