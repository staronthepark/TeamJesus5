// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectTextUI.h"

#include "UserSettingSelectUI.generated.h"

/**
 * 
 */
enum class Language :uint8;

UCLASS()
class SERVAME_API UUserSettingSelectUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	USelectTextUI* WBP_Setting_SelectText_Camera;		
	
	UPROPERTY(meta = (BindWidget))
	USelectTextUI* WBP_Setting_SelectText_Audio;		
	
/*	UPROPERTY(meta = (BindWidget))
	USelectTextUI* WBP_Setting_SelectText_Key;	*/	
	
	//UPROPERTY(meta = (BindWidget))
	//USelectTextUI* WBP_Setting_SelectText_Graphics;

	UPROPERTY(meta = (BindWidget))
	USelectTextUI* WBP_Setting_SelectText_Quit;

	UPROPERTY(meta = (BindWidget))
	USelectTextUI* WBP_Setting_SelectText_Keyguide;

	void ChangeLanguage(Language& language);

};
