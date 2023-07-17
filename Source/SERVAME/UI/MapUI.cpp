// Fill out your copyright notice in the Description page of Project Settings.


#include "MapUI.h"


void UMapUI::PlayMapInfoAnimation(FString MapName, bool isOpen)
{
	PlayAnimation(MapInfoOpenAnimation, isOpen ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
}
