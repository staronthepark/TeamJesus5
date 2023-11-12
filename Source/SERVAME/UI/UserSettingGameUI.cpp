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

	SelectArray.Add(WBP_Setting_Slider);
	//SelectArray.Add(WBP_Language_Button);
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

	index = 0;
	LeftRightIndex = 0;
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
		GameInstance->SetLanguage(Language::KOR);
	else
		GameInstance->SetLanguage(Language::ENG);

	if (GameInstance->language == Language::ENG)
	{
		//TitleGameImage->SetBrushFromTexture(ImageTextures.Find(EGameSettings::title)->EngTexture, true);
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
		//TitleGameImage->SetBrushFromTexture(ImageTextures.Find(EGameSettings::title)->KorTexture, true);
		//CameraImage->SetBrushFromTexture(ImageTextures.Find(EGameSettings::camera)->KorTexture, true);
		MouseSensitiveImage->SetBrushFromTexture(ImageTextures.Find(EGameSettings::mouse)->KorTexture, true);
		LanguageImage->SetBrushFromTexture(ImageTextures.Find(EGameSettings::language)->KorTexture, true);
		LightSettingButton->WidgetStyle.Normal.SetResourceObject(ImageTextures.Find(EGameSettings::brightnessnormal)->KorTexture);
		LightSettingButton->WidgetStyle.Hovered.SetResourceObject(ImageTextures.Find(EGameSettings::brightnesshovered)->KorTexture);
		LightSettingButton->WidgetStyle.Pressed.SetResourceObject(ImageTextures.Find(EGameSettings::brightnesspressed)->KorTexture);
		//WBP_Camera_Button->ChangeLanguage();
	}
}

FReply UUserSettingGameUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();

	if (InKeyEvent.GetKey() == EKeys::Down)
	{
		index = FMath::Clamp(index + 1, 0, SelectArray.Num() - 1);
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Up)
	{
		index = FMath::Clamp(index - 1, 0, SelectArray.Num() - 1);
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Right)
	{
		if (index == 1)
			LeftRightIndex = FMath::Clamp(LeftRightIndex + 1, 0, 1);
		else
		{
			Cast<USliderUI>(SelectArray[index])->SetValue(Cast<USliderUI>(SelectArray[index])->Slider_1->GetValue() + 0.05f);
		}
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Left)
	{
		if(index ==  1)
			LeftRightIndex = FMath::Clamp(LeftRightIndex - 1, 0, 1);
		else
		{
			Cast<USliderUI>(SelectArray[index])->SetValue(Cast<USliderUI>(SelectArray[index])->Slider_1->GetValue() - 0.05f);
		}
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Enter)
	{
		if (index == 1)
		{
			if (LeftRightIndex == 0)
				Cast<UButtonUI>(SelectArray[index])->LeftButton->OnClicked.Broadcast();
			else
				Cast<UButtonUI>(SelectArray[index])->RightButton->OnClicked.Broadcast();
		}
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::Platform_Delete)
	{
		GetParent()->SetKeyboardFocus();
		Reply = FReply::Handled();
	}
	return Reply;
}

