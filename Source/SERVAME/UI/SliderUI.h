// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/ProgressBar.h>
#include <Components/Slider.h>

#include "SliderUI.generated.h"
/**
 * 
 */
UCLASS()
class SERVAME_API USliderUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UProgressBar* ProgressBar_1;

	UFUNCTION()
	void SetProgressBarValue(float value);

public:
	UPROPERTY(meta = (BindWidget))
		USlider* Slider_1;
	virtual void NativeOnInitialized() override;

	void SetValue(float value);
};
