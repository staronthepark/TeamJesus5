// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/ProgressBar.h>
#include "Components/TextBlock.h"
#include "BossCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBossCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PB_BossHP;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BossHP;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BossGroggyGage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Distance;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AccumulateDamage;
};
