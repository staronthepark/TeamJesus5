// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettingSelectUI.h"
#include <SERVAME/Manager/JesusGameInstance.h>

void UUserSettingSelectUI::ChangeLanguage()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	WBP_Setting_SelectText_Camera->ChangeLanguage();
	WBP_Setting_SelectText_Audio->ChangeLanguage();
	WBP_Setting_SelectText_Graphics->ChangeLanguage();
	WBP_Setting_SelectText_Quit->ChangeLanguage();
}
