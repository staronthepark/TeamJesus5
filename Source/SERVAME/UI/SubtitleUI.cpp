// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtitleUI.h"
#include "Components/Image.h"

void USubtitleUI::PlaySubtitle(EGuides value)
{
	SubtitleImage->SetBrushFromTexture(Subtitles.Find(value)->Texture, true);
	PlayAnimation(StartSubtitleAnimation);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(DelayTimerHandle))
		TimerManager.ClearTimer(DelayTimerHandle);

	TimerManager.SetTimer(DelayTimerHandle, [&]() {
		PlayAnimation(EndSubtitleAnimation);
		}, Subtitles.Find(value)->Endtime, false);
}
