// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressiveBarUI.h"
#include "Components/CanvasPanelSlot.h"



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

void UProgressiveBarUI::IncreaseLength(float value)
{
	float x = Cast<UCanvasPanelSlot>(Bar->Slot)->GetSize().X;
	Cast<UCanvasPanelSlot>(Bar->Slot)->SetSize(FVector2D(x * value, 14.0f));
	Cast<UCanvasPanelSlot>(Bar_Y->Slot)->SetSize(FVector2D(x * value, 14.0f));
	Cast<UCanvasPanelSlot>(White_P->Slot)->SetSize(FVector2D(x * value, 14.0f));
}
