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

	WBP_Reflect_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetReflect);
	WBP_Reflect_Button->RightButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetReflect);

	Setting = GEngine->GetGameUserSettings();
	
	WBP_Volumetric_Button->SetValue(Setting->GetPostProcessingQuality() - 1);
	WBP_Shadow_Button->SetValue(Setting->GetShadingQuality() - 1);
	WBP_GI_Button->SetValue(Setting->GetGlobalIlluminationQuality() - 1);
	WBP_Effect_Button->SetValue(Setting->GetVisualEffectQuality() - 2);
	WBP_Reflect_Button->SetValue(Setting->GetReflectionQuality() - 1);
}

void UUserSetting_GraphicsUI::NativeConstruct()
{
	Super::NativeConstruct();
	ChangeLanguage();
}

void UUserSetting_GraphicsUI::SetVolumetic()
{
	Setting->SetPostProcessingQuality(WBP_Volumetric_Button->GetValue() + 1);
	Setting->ApplySettings(true);
}

void UUserSetting_GraphicsUI::SetShadow()
{
	Setting->SetShadowQuality(WBP_Shadow_Button->GetValue() + 1);
	Setting->ApplySettings(true);
}

void UUserSetting_GraphicsUI::SetGI()
{
	Setting->SetGlobalIlluminationQuality(WBP_GI_Button->GetValue() + 1);
	Setting->ApplySettings(true);
}

void UUserSetting_GraphicsUI::SetEffect()
{
	Setting->SetVisualEffectQuality(WBP_Effect_Button->GetValue() + 2);
	Setting->ApplySettings(true);
}

void UUserSetting_GraphicsUI::SetReflect()
{
	Setting->SetReflectionQuality(WBP_Reflect_Button->GetValue() + 1);
	Setting->ApplySettings(true);
}



void UUserSetting_GraphicsUI::ChangeLanguage()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->language == Language::ENG)
	{
		//TitleGraphicImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::title)->EngTexture, true);
		VolumetricImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->EngTexture, true);
		ShadowImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::shadow)->EngTexture, true);
		GIImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::Gi)->EngTexture, true);
		EffectImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::effect)->EngTexture, true);
		ReflectImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::reflect)->EngTexture, true);
	}
	else if (GameInstance->language == Language::KOR)
	{
		//TitleGraphicImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::title)->KorTexture, true);
		VolumetricImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->KorTexture, true);
		ShadowImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::shadow)->KorTexture, true);
		GIImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::Gi)->KorTexture, true);
		EffectImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::effect)->KorTexture, true);
		ReflectImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::reflect)->KorTexture, true);
	}

	WBP_Volumetric_Button->ChangeLanguage();
	WBP_Shadow_Button->ChangeLanguage();
	WBP_GI_Button->ChangeLanguage();
	WBP_Effect_Button->ChangeLanguage();
	WBP_Reflect_Button->ChangeLanguage();
}
