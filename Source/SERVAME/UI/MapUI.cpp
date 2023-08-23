// Fill out your copyright notice in the Description page of Project Settings.


#include "MapUI.h"
#include <SERVAME/Manager/JesusGameInstance.h>


void UMapUI::PlayMapInfoAnimation(EMapName MapName, bool isOpen)
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->language == Language::ENG)
	{
		MapInformation->SetBrushFromTexture(Textures.Find(MapName)->EngTextures, true);
	}
	else if (GameInstance->language == Language::KOR)
	{
		MapInformation->SetBrushFromTexture(Textures.Find(MapName)->KorTextures, true);
	}
	PlayAnimation(MapInfoOpenAnimation, isOpen ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
}
