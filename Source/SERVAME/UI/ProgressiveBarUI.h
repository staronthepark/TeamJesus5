// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUI.h"
#include <Components/Slider.h>
#include "ProgressiveBarUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UProgressiveBarUI : public UBaseUI
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UProgressBar* Bar;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* Bar_Y;

	UPROPERTY(meta = (BindWidget))
		USlider* White_P;

	FTimerHandle DelayTimerHandle;
	FTimerHandle ProgressTimerHandle;

public:
	void SetValue(float value);
	void DecreaseGradual(AActor* Actor, float value);

	void IncreaseLength(float value);
};
