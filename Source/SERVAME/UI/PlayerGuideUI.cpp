// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGuideUI.h"
#include <Kismet/GameplayStatics.h>


void UPlayerGuideUI::SetGamePad(int isPad)
{
	isGamePad = isPad;
}

void UPlayerGuideUI::StartGuide(EGuides type)
{
	if (GuideTextures.Find(type)->isRenderedOnce == true)
		return;
	IsRenderImage = true;
	UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
	this->SetRenderOpacity(1.0f);
	GuideImage->SetBrushFromTexture(GuideTextures.Find(type)->Textures[isGamePad]);
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




