// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "StatMarkUI.generated.h"

/**
 * 
 */

UENUM()
enum class EState : uint8
{
	Deactive	UMETA(DisplayName = "Deactive"),
	CanActive	UMETA(DisplayName = "CanActive"),
	Active		UMETA(DisplayName = "Active")
};

UENUM()
enum class EStat : uint8
{
	stength		UMETA(DisplayName = "Strength"),
	stamina		UMETA(DisplayName = "Stamina"),
	hp			UMETA(DisplayName = "Hp")
};

UCLASS()
class SERVAME_API UStatMarkUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EState State;

	UPROPERTY(EditAnywhere)
	EStat Stat;

	UPROPERTY(EditAnywhere)
	TArray<UStatMarkUI*> NextMarks;

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	void SetStat(EState value);

	UFUNCTION()
	void OnClicked();

};
