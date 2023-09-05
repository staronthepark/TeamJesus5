// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "../Player/PlayerStatComponent.h"
#include "StatUI.generated.h"

/**
 * 
 */

UENUM()
enum class EStatState : uint8
{
	closed		UMETA(DisplayName = "Closed"),
	can			UMETA(DisplayName = "CanActivate"),
	activated	UMETA(DisplayName = "Activated")
};

UENUM()
enum class EStateType : uint8
{
	str		UMETA(DisplayName = "Strength"),
	stamina	UMETA(DisplayName = "Stamina"),
	hp		UMETA(DiaplayName = "Hp"),
	shield	UMETA(DisplayName = "Shield")
};

UCLASS()
class SERVAME_API UStatUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	UPROPERTY(EditAnywhere)
	EStatState state;

	UPROPERTY(EditAnywhere)
	UStatUI* NextStat;

	UPROPERTY(EditAnywhere)
	EStateType Type;

	TMap<EStateType, TFunction<void(int32 index)>> TypeAnimation;

	UPlayerStatComponent* PlayerStatComp;
	
public:
	int index;

	virtual void NativeOnInitialized();
	void ChangeState(EStatState changeState);


	UFUNCTION()
	void OnButtonClicked();
};
