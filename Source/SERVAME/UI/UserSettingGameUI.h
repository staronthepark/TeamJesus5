// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubUserSettingUI.h"
#include "SliderUI.h"
#include "ButtonUI.h"
#include "UserSettingLightUI.h"

#include "UserSettingGameUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UUserSettingGameUI : public USubUserSettingUI
{
	GENERATED_BODY()

		//UPROPERTY(meta = (BindWidget))
		//	UButtonUI* WBP_Setting_Button;

	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_Camera_Button;

	UPROPERTY(meta = (BindWidget))
	USliderUI* WBP_Setting_Slider;

	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_Language_Button;


	//UPROPERTY(meta = (BindWidget))
	//	UUserSettingLightUI* WBP_UserSetting_LightUI;
	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UUserSettingLightUI> UserSettingLightUIClass;
	//UUserSettingLightUI* UserSettingLightUI;

public:
	UPROPERTY(meta = (BindWidget))
		UButton* LightSettingButton;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//UFUNCTION()
	//	void ClickLightSettingButton();

	//UFUNCTION()
	//	void SetBleeding();
	UFUNCTION()
		void SetMouseSensitive(float value);

	UFUNCTION()
		void ChangeCameraView();

	UFUNCTION()
		void SetLanguage();
};
