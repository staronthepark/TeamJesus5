// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSetting_GraphicsUI.h"
#include <SERVAME/Manager/JesusGameInstance.h>

void UUserSetting_GraphicsUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUserSetting_GraphicsUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUserSetting_GraphicsUI::SetVolumetic()
{
}

void UUserSetting_GraphicsUI::SetShadow()
{
}

void UUserSetting_GraphicsUI::SetGI()
{
}

void UUserSetting_GraphicsUI::SetEffect()
{
}

void UUserSetting_GraphicsUI::ChangeLanguage()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->language == Language::ENG)
	{
		VolumetricImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->EngTexture, true);
		ShadowImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->EngTexture, true);
		GIImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->EngTexture, true);
		EffectImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->EngTexture, true);
	}
	else if (GameInstance->language == Language::KOR)
	{
		VolumetricImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->KorTexture, true);
		ShadowImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->KorTexture, true);
		GIImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->KorTexture, true);
		EffectImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->KorTexture, true);
	}
}
