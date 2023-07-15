// Fill out your copyright notice in the Description page of Project Settings.


#include "SliderUI.h"

void USliderUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Slider_1->OnValueChanged.AddDynamic(this, &USliderUI::SetProgressBarValue);
}

void USliderUI::SetProgressBarValue(float value)
{
	ProgressBar_1->SetPercent(value);
}

void USliderUI::SetValue(float value)
{
	Slider_1->SetValue(value);
	ProgressBar_1->SetPercent(value);
}
