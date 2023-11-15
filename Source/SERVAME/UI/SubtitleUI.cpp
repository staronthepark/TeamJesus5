// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtitleUI.h"
#include "Components/Image.h"

void USubtitleUI::PlaySubtitle(EGuides value)
{
	guides = value;
	SubtitleImage->SetBrushFromTexture(Subtitles.Find(value)->Texture[index], true);
	PlayAnimation(StartSubtitleAnimation);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(DelayTimerHandle))
		TimerManager.ClearTimer(DelayTimerHandle);

	TimerManager.SetTimer(DelayTimerHandle, [&]() {
		index++;
		if (index < Subtitles.Find(guides)->Texture.Num()) {
			PlaySubtitle(guides);
		}
		else
		{
			index = 0;
			PlayAnimation(EndSubtitleAnimation);
		}
		}, Subtitles.Find(value)->Endtime[index], false);
}

void USubtitleUI::NativeConstruct()
{
	index = 0;
}
