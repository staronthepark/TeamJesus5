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

enum class Language :uint8;

UENUM()
enum class EGraphicsSettings : uint8
{
	title		UMETA(DisplayName = "Title"),
	volumetric	UMETA(DisplayName = "Volumetric"),
	shadow		UMETA(DisplayName = "Shadow"),
	Gi			UMETA(DisplayName = "GI"),
	effect		UMETA(DisplayName = "Effect"),
	reflect		UMETA(DiplayName = "Reflect")
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
	//UPROPERTY(meta = (BindWidget))
	//	UImage* TitleGraphicImage;

	UPROPERTY(meta = (BindWidget))
		UImage* VolumetricImage;

	UPROPERTY(meta = (BindWidget))
		UImage* ShadowImage;

	UPROPERTY(meta = (BindWidget))
		UImage* GIImage;

	UPROPERTY(meta = (BindWidget))
		UImage* EffectImage;

	UPROPERTY(meta = (BindWidget))
		UImage* ReflectImage;


	UPROPERTY(EditAnywhere)
	TMap<EGraphicsSettings, FGraphicsTextures> ImageTextures;

	UGameUserSettings* Setting;

public:
	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_Volumetric_Button;	
	
	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_Shadow_Button;
	
	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_GI_Button;
	
	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_Effect_Button;

	UPROPERTY(meta = (BindWidget))
	UButtonUI* WBP_Reflect_Button;

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
		void SetReflect();

	UFUNCTION()
		void ChangeLanguage(Language& language);


};
