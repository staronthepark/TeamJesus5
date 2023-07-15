// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/TextBlock.h>
#include "Blueprint/UserWidget.h"
#include "DebugLogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UDebugLogWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	bool IsVisible;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_PlayerHP;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_PlayerStamina;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_BossHP;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_BossGroggyGauage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_Distance;
};
