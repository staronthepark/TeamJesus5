// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressiveBarUI.h"



void UProgressiveBarUI::SetValue(float value)
{
	value = FMath::Clamp(value, 0.0f, 1.0f);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	Bar->SetPercent(value);
	White_P->SetValue(value);
	if (!TimerManager.IsTimerActive(DelayTimerHandle) && !TimerManager.IsTimerActive(ProgressTimerHandle))
		Bar_Y->SetPercent(value);
}

void UProgressiveBarUI::DecreaseGradual(AActor* Actor, float value)
{
	value = FMath::Clamp(value, 0.0f, 1.0f);

	Bar->SetPercent(value);
	White_P->SetValue(value);
	ProgressDecrease(Actor, Bar_Y, value, 1, DelayTimerHandle, ProgressTimerHandle);
}
