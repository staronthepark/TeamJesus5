// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SliderUI.h"
#include <Components/Image.h>
#include "UserSettingLightUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UUserSettingLightUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* LightSettingImage;

	UPROPERTY(meta = (BindWidget))
		USliderUI* WBP_Setting_Slider;

	UPROPERTY(meta = (BindWidget))
		UImage* RightImage;

	UPROPERTY(EditAnywhere)
		TArray<UTexture2D*> LightSettingTextures;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	UFUNCTION()
		void ChangeSliderValue(float value);

	UFUNCTION()
		void ChangeLanguage();
};
