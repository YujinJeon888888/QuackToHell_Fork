// Copyright_Team_AriAri

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GodFunction.h"
#include "GodCall.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUACKTOHELL_API UGodCall : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGodCall();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// GodFunction 호출
	UFUNCTION(BlueprintCallable, Category = "God")
	void StartGodProcess();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
