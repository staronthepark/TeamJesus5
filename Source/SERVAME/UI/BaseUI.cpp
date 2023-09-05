// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUI.h"


void UBaseUI::ProgressDecrease(AActor* Actor,UProgressBar* ProgressBar, float _min, float _max, FTimerHandle& DelayTimerHandle, FTimerHandle& ProgressTimerHandle)
{
	if(TempProgressBar != ProgressBar)
		TempProgressBar = ProgressBar;
	min = _min;
	max = _max;

	FTimerManager& TimerManager = Actor->GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(DelayTimerHandle))
		TimerManager.ClearTimer(DelayTimerHandle);

	TimerManager.SetTimer(DelayTimerHandle, [&]() {
		TimerManager.SetTimer(ProgressTimerHandle, [&]() {
			TempProgressBar->SetPercent(TempProgressBar->Percent -= 0.01f);
			if (TempProgressBar->Percent < min || TempProgressBar->Percent > max)
				TimerManager.ClearTimer(ProgressTimerHandle);
			}, 0.01f, true);
		}, 1.0f, false);
}

void UBaseUI::ProgressIncrease(AActor* Actor, UProgressBar* ProgressBar, float _min, float _max, FTimerHandle& DelayTimerHandle, FTimerHandle& ProgressTimerHandle)
{
	if (TempProgressBar != ProgressBar)
		TempProgressBar = ProgressBar;
	min = _min;
	max = _max;

	FTimerManager& TimerManager = Actor->GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(DelayTimerHandle))
		TimerManager.ClearTimer(DelayTimerHandle);

	TimerManager.SetTimer(DelayTimerHandle, [&]() {
		TimerManager.SetTimer(ProgressTimerHandle, [&]() {
			TempProgressBar->SetPercent(TempProgressBar->Percent += 0.01f);
			if (TempProgressBar->Percent < min || TempProgressBar->Percent > max)
				TimerManager.ClearTimer(ProgressTimerHandle);
			}, 0.01f, true);
		}, 1.0f, false);
}
