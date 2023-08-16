// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubUserSettingUI.h"
#include <Components/Image.h>
#include "GameFramework/GameUserSettings.h"
#include "SliderUI.h"
#include "ButtonUI.h"
#include "UserSetting_GraphicsUI.generated.h"

/**
 * 
 */

UENUM()
enum class EGraphicsSettings : uint8
{
	volumetric	UMETA(DisplayName = "Volumetric"),
	shadow		UMETA(DisplayName = "Shadow"),
	Gi			UMETA(DisplayName = "GI"),
	effect		UMETA(DisplayName = "Effect")
};

USTRUCT(Blueprinttype)
struct SERVAME_API FGraphicsTextures
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere)
		UTexture2D* KorTexture;
	UPROPERTY(EditAnywhere)
		UTexture2D* EngTexture;
};

UCLASS()
class SERVAME_API UUserSetting_GraphicsUI : public USubUserSettingUI
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* VolumetricImage;

	UPROPERTY(meta = (BindWidget))
		UImage* ShadowImage;

	UPROPERTY(meta = (BindWidget))
		UImage* GIImage;

	UPROPERTY(meta = (BindWidget))
		UImage* EffectImage;

	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_Volumetric_Button;	
	
	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_Shadow_Button;
	
	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_GI_Button;
	
	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_Effect_Button;

	UPROPERTY(EditAnywhere)
	TMap<EGraphicsSettings, FGraphicsTextures> ImageTextures;

	UGameUserSettings* Setting;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
		void SetVolumetic();

	UFUNCTION()
		void SetShadow();

	UFUNCTION()
		void SetGI();

	UFUNCTION()
		void SetEffect();

	UFUNCTION()
		void ChangeLanguage();


};
