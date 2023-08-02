// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettingSelectUI.h"

void UUserSettingSelectUI::ChangeLanguage()
{
	WBP_Setting_SelectText_Camera->ChangeLanguage();
	WBP_Setting_SelectText_Audio->ChangeLanguage();
	WBP_Setting_SelectText_Quit->ChangeLanguage();
}
