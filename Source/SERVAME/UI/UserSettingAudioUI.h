// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubUserSettingUI.h"
#include "SliderUI.h"
#include "ButtonUI.h"

#include "UserSettingAudioUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UUserSettingAudioUI : public USubUserSettingUI
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_Setting_Button;

	UPROPERTY(meta = (BindWidget))
	USliderUI* WBP_Setting_Slider_BGM;

	UPROPERTY(meta = (BindWidget))
	USliderUI* WBP_Setting_Slider_SE;

	UPROPERTY(meta = (BindWidget))
	USliderUI* WBP_Setting_Slider_Voice;


public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetMuteSound();
	UFUNCTION()
	void SetBGMVolume(float value);
	UFUNCTION()
	void SetSFXVolume(float value);
	UFUNCTION()
	void SetVoiceVolume(float value);

};
