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

enum class Language :uint8;

UENUM()
enum class EAudioSettings : uint8
{
	title	UMETA(DisplayName = "Title"),
	master	UMETA(DisplayName = "Master"),
	bgm		UMETA(DisplayName = "BGM"),
	se		UMETA(DisplayName = "SE"),
	//voice	UMETA(DisplayName = "Voice")
};

USTRUCT(BlueprintType)
struct SERVAME_API FAudioTextures
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere)
		UTexture2D* KorTexture;
	UPROPERTY(EditAnywhere)
		UTexture2D* EngTexture;
};

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

	//UPROPERTY(meta = (BindWidget))
	//USliderUI* WBP_Setting_Slider_Voice;

	//UPROPERTY(meta = (BindWidget))
	//UImage* TitleAudioImage;

	UPROPERTY(meta = (BindWidget))
	UImage* MasterAudioImage;

	UPROPERTY(meta = (BindWidget))
	UImage* BGMAudioImage;	
	
	UPROPERTY(meta = (BindWidget))
	UImage* SEAudioImage;
	
	//UPROPERTY(meta = (BindWidget))
	//UImage* VoiceAudioImage;

	UPROPERTY(EditAnywhere)
		TMap<EAudioSettings, FAudioTextures> ImageTextures;


public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetMuteSound();
	UFUNCTION()
	void SetBGMVolume(float value);
	UFUNCTION()
	void SetSFXVolume(float value);
	//UFUNCTION()
	//void SetVoiceVolume(float value);

	UFUNCTION()
	void ChangeLanguage(Language& language);

};
