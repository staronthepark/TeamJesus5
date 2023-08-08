// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGuideUI.h"
#include <SERVAME/Manager/JesusGameInstance.h>
#include <Kismet/GameplayStatics.h>


void UPlayerGuideUI::SetGamePad(int isPad)
{
	isGamePad = isPad;
}

void UPlayerGuideUI::StartGuide(EGuides type, Language language)
{
	if (GuideTextures.Find(type)->isRenderedOnce == true)
		return;
	IsRenderImage = true;
	UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
	this->SetRenderOpacity(1.0f);
	if(language == Language::KOR)
		GuideImage->SetBrushFromTexture(GuideTextures.Find(type)->KorTextures[isGamePad]);
	else if(language == Language::ENG)
		GuideImage->SetBrushFromTexture(GuideTextures.Find(type)->EngTextures[isGamePad]);

	GuideTextures.Find(type)->isRenderedOnce = true;
}

bool UPlayerGuideUI::IsRender()
{
	return IsRenderImage;
}

void UPlayerGuideUI::Disable()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
	IsRenderImage = false;
	this->SetRenderOpacity(0.0f);
}




