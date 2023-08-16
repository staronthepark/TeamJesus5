// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSetting_GraphicsUI.h"
#include <SERVAME/Manager/JesusGameInstance.h>

void UUserSetting_GraphicsUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WBP_Volumetric_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetVolumetic);
	WBP_Volumetric_Button->RightButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetVolumetic);

	WBP_Shadow_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetShadow);
	WBP_Shadow_Button->RightButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetShadow);

	WBP_GI_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetGI);
	WBP_GI_Button->RightButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetGI);

	WBP_Effect_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetEffect);
	WBP_Effect_Button->RightButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetEffect);

	Setting = GEngine->GetGameUserSettings();
}

void UUserSetting_GraphicsUI::NativeConstruct()
{
	Super::NativeConstruct();
	ChangeLanguage();
}

void UUserSetting_GraphicsUI::SetVolumetic()
{
	Setting->SetPostProcessingQuality(WBP_Volumetric_Button->index + 1);
	Setting->ApplySettings(true);
}

void UUserSetting_GraphicsUI::SetShadow()
{
	Setting->SetShadowQuality(WBP_Shadow_Button->index + 1);
	Setting->ApplySettings(true);
}

void UUserSetting_GraphicsUI::SetGI()
{
	Setting->SetGlobalIlluminationQuality(WBP_GI_Button->index + 1);
	Setting->ApplySettings(true);
}

void UUserSetting_GraphicsUI::SetEffect()
{
	Setting->SetVisualEffectQuality(WBP_Effect_Button->index + 2);
	Setting->ApplySettings(true);
}

void UUserSetting_GraphicsUI::ChangeLanguage()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->language == Language::ENG)
	{
		VolumetricImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->EngTexture, true);
		ShadowImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::shadow)->EngTexture, true);
		GIImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::Gi)->EngTexture, true);
		EffectImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::effect)->EngTexture, true);
	}
	else if (GameInstance->language == Language::KOR)
	{
		VolumetricImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->KorTexture, true);
		ShadowImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::shadow)->KorTexture, true);
		GIImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::Gi)->KorTexture, true);
		EffectImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::effect)->KorTexture, true);
	}
}
