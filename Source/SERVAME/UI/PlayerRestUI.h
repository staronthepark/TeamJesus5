// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include <SERVAME/Manager/JesusGameInstance.h>
#include "PlayerRestUI.generated.h"

/**
 * 
 */

UENUM()
enum class ERestButtons : uint8
{
	PrayNormal	UMETA(DisplayName = "PrayNormal"),
	PrayHovered	UMETA(DisplayName = "PrayHovered"),
	//PrayPressed	UMETA(DisplayName = "PrayPressed"),
	SoulNormal	UMETA(DisplayName = "SoulNormal"),
	SoulHovered	UMETA(DisplayName = "SoulHovered"),
	//SoulPressed	UMETA(DisplayName = "SoulPressed")
};

USTRUCT(BlueprintType)
struct SERVAME_API FRestLanguageTextures
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	UTexture2D* KorTextures;

	UPROPERTY(EditAnywhere)
	UTexture2D* EngTextures;
};


UCLASS()
class SERVAME_API UPlayerRestUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UImage* PrayImage;	
	
	UPROPERTY(meta = (BindWidget))
	UImage* SoulImage;
	
	UPROPERTY(meta = (BindWidget))
	UImage* PrayBackgroundHovered;

	UPROPERTY(meta = (BindWidget))
	UImage* SoulBackgroundHovered;

	UPROPERTY(EditAnywhere)
	TMap<ERestButtons, FRestLanguageTextures> ButtonTextures;

	Language currentLanguage;

public:
	UPROPERTY(meta = (BindWidget))
	UButton* PrayButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SoulButton;

	virtual void NativeOnInitialized();

	UFUNCTION()
	void OnPrayButtonUnhovered();	
	UFUNCTION()
	void OnPrayButtonHovered();	
	UFUNCTION()
	void OnSoulButtonUnhovered();
	UFUNCTION()
	void OnSoulButtonHovered();

	UFUNCTION()
	void OnPrayButtonClicked();

	UFUNCTION()
	void OnSoulButtonClicked();

	UFUNCTION()
	void Open();
};
