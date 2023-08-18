// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettingGameUI.h"
#include "../Player/JesusPlayerController.h"
#include "..\Manager\JesusGameInstance.h"



void UUserSettingGameUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//WBP_Setting_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSettingGameUI::SetBleeding);
	//WBP_Setting_Button->RightButton->OnClicked.AddDynamic(this, &UUserSettingGameUI::SetBleeding);
	WBP_Setting_Slider->Slider_1->OnValueChanged.AddDynamic(this, &UUserSettingGameUI::SetMouseSensitive);
	//WBP_Camera_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSettingGameUI::ChangeCameraView);
	//WBP_Camera_Button->RightButton->OnClicked.AddDynamic(this, &UUserSettingGameUI::ChangeCameraView);

	WBP_Language_Button->RightButton->OnClicked.AddDynamic(this, &UUserSettingGameUI::SetLanguage);
	WBP_Language_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSettingGameUI::SetLanguage);

	//LightSettingButton->OnClicked.AddDynamic(this, &UUserSettingGameUI::ClickLightSettingButton);
}

void UUserSettingGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	//UserSettingLightUI = Cast<UUserSettingLightUI>(CreateWidget(GetWorld(), UserSettingLightUIClass));
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (GameInstance->PlayerOptionSetting.Bleeding == true)
	//	WBP_Setting_Button->SetValue(0);
	//else
	//	WBP_Setting_Button->SetValue(1);

	SetLanguage();

	WBP_Setting_Slider->SetValue((GameInstance->PlayerOptionSetting.DPI - 20) / 40);

}

void UUserSettingGameUI::NativeDestruct()
{
	Super::NativeDestruct();
	//WBP_UserSetting_LightUI->SetVisibility(ESlateVisibility::Collapsed);
}

//void UUserSettingGameUI::ClickLightSettingButton()
//{
//	WBP_UserSetting_LightUI->SetVisibility(ESlateVisibility::Visible);
//}

//void UUserSettingGameUI::SetBleeding()
//{
//	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//	if (WBP_Setting_Button->GetValue() == 0)
//		GameInstance->PlayerOptionSetting.Bleeding = true;
//	else
//		GameInstance->PlayerOptionSetting.Bleeding = false;
//}

void UUserSettingGameUI::SetMouseSensitive(float value)
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->PlayerOptionSetting.DPI = (value * 40) + 20;
}

//void UUserSettingGameUI::ChangeCameraView()
//{
//	if(WBP_Camera_Button->GetValue() == 0)
//		Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController())->character->ShoulderView(true);
//	else
//		Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController())->character->ShoulderView(false);
//}

void UUserSettingGameUI::SetLanguage()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (WBP_Language_Button->GetValue() == 0)
		GameInstance->SetLanguage(Language::ENG);
	else
		GameInstance->SetLanguage(Language::KOR);

	if (GameInstance->language == Language::ENG)
	{
		TitleGameImage->SetBrushFromTexture(ImageTextures.Find(EGameSettings::title)->EngTexture, true);
		//CameraImage->SetBrushFromTexture(ImageTextures.Find(EGameSettings::camera)->EngTexture, true);
		MouseSensitiveImage->SetBrushFromTexture(ImageTextures.Find(EGameSettings::mouse)->EngTexture, true);
		LanguageImage->SetBrushFromTexture(ImageTextures.Find(EGameSettings::language)->EngTexture, true);
		LightSettingButton->WidgetStyle.Normal.SetResourceObject(ImageTextures.Find(EGameSettings::brightnessnormal)->EngTexture);
		LightSettingButton->WidgetStyle.Hovered.SetResourceObject(ImageTextures.Find(EGameSettings::brightnesshovered)->EngTexture);
		LightSettingButton->WidgetStyle.Pressed.SetResourceObject(ImageTextures.Find(EGameSettings::brightnesspressed)->EngTexture);
		//WBP_Camera_Button->ChangeLanguage();
	}
	else if (GameInstance->language == Language::KOR)
	{
		TitleGameImage->SetBrushFromTexture(ImageTextures.Find(EGameSettings::title)->KorTexture, true);
		//CameraImage->SetBrushFromTexture(ImageTextures.Find(EGameSettings::camera)->KorTexture, true);
		MouseSensitiveImage->SetBrushFromTexture(ImageTextures.Find(EGameSettings::mouse)->KorTexture, true);
		LanguageImage->SetBrushFromTexture(ImageTextures.Find(EGameSettings::language)->KorTexture, true);
		LightSettingButton->WidgetStyle.Normal.SetResourceObject(ImageTextures.Find(EGameSettings::brightnessnormal)->KorTexture);
		LightSettingButton->WidgetStyle.Hovered.SetResourceObject(ImageTextures.Find(EGameSettings::brightnesshovered)->KorTexture);
		LightSettingButton->WidgetStyle.Pressed.SetResourceObject(ImageTextures.Find(EGameSettings::brightnesspressed)->KorTexture);
		//WBP_Camera_Button->ChangeLanguage();
	}

	
}

