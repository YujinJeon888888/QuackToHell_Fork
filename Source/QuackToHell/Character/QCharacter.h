// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QCharacter.generated.h"

UCLASS()
class QUACKTOHELL_API AQCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AQCharacter();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
protected:
	/**
	 * @brief 이름 리턴 함수
	 * @return 이름
	 */
	const FString& GetCharacterName() const;
	/**
	 * @brief 이름 설정 함수
	 * @return 이름
	 */
	void SetCharacterName(FString& Name);

	/**
	 * @brief UI 컴포넌트입니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UWidgetComponent> WidgetComponent;
	/** @brief 네임위젯 클래스 정보를 담습니다. */
	UPROPERTY()
	TObjectPtr<class UQNameWidget> NameWidget;
	/** @brief 네임위젯에 이름을 반영하는 함수를 오버라이드하도록 강제함 */
	virtual void NameToNameWidget() PURE_VIRTUAL(AQCharacter::NameToNameWidget, );

private:
	/**
	 * @brief 이름
	 */
	FString CharacterName;
};
