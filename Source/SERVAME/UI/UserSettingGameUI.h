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

UENUM()
enum class EGameSettings : uint8
{
	title		UMETA(DisplayName = "Title"),
	camera		UMETA(DisplayName = "Camera"),
	mouse		UMETA(DisplayName = "Mouse"),
	brightnessnormal	UMETA(DisplayName = "BrightnessNormal"),
	brightnesshovered	UMETA(DisplayName = "BrightnessHovered"),
	brightnesspressed	UMETA(DisplayName = "BrightnessPressed")
};

USTRUCT(BlueprintType)
struct SERVAME_API FTextures
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		UTexture2D* KorTexture;
	UPROPERTY(EditAnywhere)
		UTexture2D* EngTexture;
};

UCLASS()
class SERVAME_API UUserSettingGameUI : public USubUserSettingUI
{
	GENERATED_BODY()

		//UPROPERTY(meta = (BindWidget))
		//	UButtonUI* WBP_Setting_Button;

	UPROPERTY(meta = (BindWidget))
		UImage* TitleGameImage;

	UPROPERTY(meta = (BindWidget))
		UImage* CameraImage;

	UPROPERTY(meta = (BindWidget))
		UImage* MouseSensitiveImage;

	UPROPERTY(EditAnywhere)
	TMap<EGameSettings, FTextures> ImageTextures;


	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_Camera_Button;

	UPROPERTY(meta = (BindWidget))
	USliderUI* WBP_Setting_Slider;


	//UPROPERTY(meta = (BindWidget))
	//	UUserSettingLightUI* WBP_UserSetting_LightUI;
	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UUserSettingLightUI> UserSettingLightUIClass;
	//UUserSettingLightUI* UserSettingLightUI;

public:

	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_Language_Button;

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
