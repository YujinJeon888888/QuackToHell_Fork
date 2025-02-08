// Copyright_Team_AriAri


#include "GodCall.h"
#include "GodFunction.h"

// Sets default values for this component's properties
UGodCall::UGodCall()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGodCall::BeginPlay()
{
	Super::BeginPlay();

}

bool UGodCall::StartGodProcess()
{
    UE_LOG(LogTemp, Log, TEXT("God process started. Generating NPC Prompts..."));

    // 성공 여부를 판단할 수 있도록 bool 값 반환
    UGodFunction::GenerateNPCPrompts();

    // 파일이 정상적으로 생성되었는지 확인
    FString FilePath = FPaths::ProjectContentDir() + TEXT("Prompt/PromptToJury1.json");
    if (FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("God process successfully generated NPC prompts."));
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("God process failed to generate NPC prompts."));
        return false;
    }
}

void UGodCall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

