// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettingUI.h"
#include "..\Manager\SoundManager.h"

#define Game 0
#define Audio 1


void UUserSettingUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//GameExitUI = Cast<UGameExitUI>(CreateWidget(GetWorld(), GameExitUIClass));
	SelectSettingArray.Add(WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Camera); //0
	SelectSettingArray.Add(WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Audio); //1
	SelectSettingArray.Add(WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Quit); //2

	SubUserSettingArray.Add(WBP_UserSetting_GameUI); // 0
	SubUserSettingArray.Add(WBP_UserSetting_AudioUI); // 1

	SelectSettingArray[0]->Button->OnClicked.AddDynamic(this, &UUserSettingUI::ClickGameSettingButton);
	SelectSettingArray[1]->Button->OnClicked.AddDynamic(this, &UUserSettingUI::ClickAudioSettingButton);
	SelectSettingArray[2]->Button->OnClicked.AddDynamic(this, &UUserSettingUI::ClickQuitSettingButton);

	WBP_UserSetting_GameUI->LightSettingButton->OnClicked.AddDynamic(this, &UUserSettingUI::ClickLightSettingButton);
}

void UUserSettingUI::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(OpenAnimation);
}

void UUserSettingUI::NativeDestruct()
{
	Super::NativeDestruct();
	UMG_GameExit->SetVisibility(ESlateVisibility::Collapsed);
	WBP_UserSetting_LightUI->SetVisibility(ESlateVisibility::Collapsed);
}

void UUserSettingUI::ClickGameSettingButton()
{
	UnselectAllButton();
	SubUserSettingArray[Game]->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(OpenGameSettingAnimation);
	WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Camera->Select();
}

void UUserSettingUI::ClickAudioSettingButton()
{
	UnselectAllButton();
	SubUserSettingArray[Audio]->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(OpenAudioSettingAnimation);
	WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Audio->Select();
}

void UUserSettingUI::ClickQuitSettingButton()
{
	UMG_GameExit->SetVisibility(ESlateVisibility::Visible);
	//if (!GameExitUI->IsInViewport())
	//{
	//	GameExitUI->AddToViewport();
	//}
}

void UUserSettingUI::ClickLightSettingButton()
{
	WBP_UserSetting_LightUI->SetVisibility(ESlateVisibility::Visible);
}


void UUserSettingUI::UnselectAllButton()
{
	for (int i = 0; i < SelectSettingArray.Num(); i++) {
		SelectSettingArray[i]->Unselect();
	}
	for (int i = 0; i < SubUserSettingArray.Num(); i++) {
		SubUserSettingArray[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}





