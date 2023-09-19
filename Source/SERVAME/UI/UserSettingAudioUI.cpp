// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettingAudioUI.h"
#include "..\Manager\SoundManager.h"
#include <SERVAME/Manager/JesusGameInstance.h>

#define On 0
#define Off 1



void UUserSettingAudioUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WBP_Setting_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSettingAudioUI::SetMuteSound);
	WBP_Setting_Button->RightButton->OnClicked.AddDynamic(this, &UUserSettingAudioUI::SetMuteSound);

	WBP_Setting_Slider_BGM->Slider_1->OnValueChanged.AddDynamic(this, &UUserSettingAudioUI::SetBGMVolume);
	WBP_Setting_Slider_SE->Slider_1->OnValueChanged.AddDynamic(this, &UUserSettingAudioUI::SetSFXVolume);
	/*WBP_Setting_Slider_Voice->Slider_1->OnValueChanged.AddDynamic(this, &UUserSettingAudioUI::SetVoiceVolume);*/
}

void UUserSettingAudioUI::NativeConstruct()
{
	Super::NativeConstruct();
	if (!ASoundManager::ASoundManager::GetInstance().IsMuteSound())
		WBP_Setting_Button->SetValue(On);
	else
		WBP_Setting_Button->SetValue(Off);
	WBP_Setting_Slider_BGM->SetValue(ASoundManager::ASoundManager::GetInstance().GetBGMVolume());
	WBP_Setting_Slider_SE->SetValue(ASoundManager::ASoundManager::GetInstance().GetSFXVolume());
	//WBP_Setting_Slider_Voice->SetValue(ASoundManager::ASoundManager::GetInstance().GetVoiceVolume());

	ChangeLanguage();
}

void UUserSettingAudioUI::SetMuteSound()
{
	if (WBP_Setting_Button->GetValue() == On) {
		ASoundManager::ASoundManager::GetInstance().MuteSound(false);		
		WBP_Setting_Slider_BGM->Slider_1->SetIsEnabled(true);
		WBP_Setting_Slider_SE->Slider_1->SetIsEnabled(true);
	}
	else {
		ASoundManager::ASoundManager::GetInstance().MuteSound(true);
		WBP_Setting_Slider_BGM->Slider_1->SetIsEnabled(false);
		WBP_Setting_Slider_SE->Slider_1->SetIsEnabled(false);
	}
	//WBP_Setting_Slider_BGM->SetValue(ASoundManager::ASoundManager::GetInstance().GetBGMVolume());
	//WBP_Setting_Slider_SE->SetValue(ASoundManager::ASoundManager::GetInstance().GetSFXVolume());
	//WBP_Setting_Slider_Voice->SetValue(ASoundManager::ASoundManager::GetInstance().GetVoiceVolume());

}

void UUserSettingAudioUI::SetBGMVolume(float value)
{
	ASoundManager::ASoundManager::GetInstance().SetBGMVolume(value);
	//ASoundManager::ASoundManager::GetInstance().MuteSound(false);
	//WBP_Setting_Button->SetValue(On);
}

void UUserSettingAudioUI::SetSFXVolume(float value)
{
	ASoundManager::ASoundManager::GetInstance().SetSFXVolume(value);
	//ASoundManager::ASoundManager::GetInstance().MuteSound(false);
	//WBP_Setting_Button->SetValue(On);
}

//void UUserSettingAudioUI::SetVoiceVolume(float value)
//{
//	ASoundManager::ASoundManager::GetInstance().SetVoiceVolume(value);
//	//ASoundManager::ASoundManager::GetInstance().MuteSound(false);
//	//WBP_Setting_Button->SetValue(On);
//}

void UUserSettingAudioUI::ChangeLanguage()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance->language == Language::ENG)
	{
		//TitleAudioImage->SetBrushFromTexture(ImageTextures.Find(EAudioSettings::title)->EngTexture, true);
		MasterAudioImage->SetBrushFromTexture(ImageTextures.Find(EAudioSettings::master)->EngTexture, true);
		BGMAudioImage->SetBrushFromTexture(ImageTextures.Find(EAudioSettings::bgm)->EngTexture, true);
		SEAudioImage->SetBrushFromTexture(ImageTextures.Find(EAudioSettings::se)->EngTexture, true);
		//VoiceAudioImage->SetBrushFromTexture(ImageTextures.Find(EAudioSettings::voice)->EngTexture, true);
	}
	else if (GameInstance->language == Language::KOR)
	{
		//TitleAudioImage->SetBrushFromTexture(ImageTextures.Find(EAudioSettings::title)->KorTexture, true);
		MasterAudioImage->SetBrushFromTexture(ImageTextures.Find(EAudioSettings::master)->KorTexture, true);
		BGMAudioImage->SetBrushFromTexture(ImageTextures.Find(EAudioSettings::bgm)->KorTexture, true);
		SEAudioImage->SetBrushFromTexture(ImageTextures.Find(EAudioSettings::se)->KorTexture, true);
		//VoiceAudioImage->SetBrushFromTexture(ImageTextures.Find(EAudioSettings::voice)->KorTexture, true);
	}
}
