// Fill out your copyright notice in the Description page of Project Settings.


#include "MapUI.h"

void UMapUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	this->SetRenderOpacity(0.f);
}

void UMapUI::PlayMapInfoAnimation(FString MapName, bool isOpen)
{
	PlayAnimation(MapInfoOpenAnimation, isOpen ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
}
