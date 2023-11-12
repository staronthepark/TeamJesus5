// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettingSelectUI.h"
#include <SERVAME/Manager/JesusGameInstance.h>

void UUserSettingSelectUI::ChangeLanguage(Language& language)
{
	WBP_Setting_SelectText_Camera->ChangeLanguage(language);
	WBP_Setting_SelectText_Audio->ChangeLanguage(language);
	WBP_Setting_SelectText_Graphics->ChangeLanguage(language);
	WBP_Setting_SelectText_Quit->ChangeLanguage(language);
}