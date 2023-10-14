// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include "PlayerGuideUI.h"
#include "MapUI.generated.h"



/**
 * 
 */

//UENUM()
//enum class EMapName : uint8
//{
//	Haros	UMETA(DisplayName = "Haros"),
//	LostWay UMETA(DisplayName = "LostWay"),
//	HouseOfAbandoned	UMETA(DisplayName = "HouseOfAbandoned"),
//	MainHall	UMETA(DisplayName = "MainHall"),
//	Repent	UMETA(DisplayName = "Repent")
//
//};

USTRUCT(BlueprintType)
struct SERVAME_API FLanguageTextures 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		UTexture2D* KorTextures;
	UPROPERTY(EditAnywhere)
		UTexture2D* EngTextures;
};

UCLASS()
class SERVAME_API UMapUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* MapInformation;

	UPROPERTY(EditAnywhere)
	TMap<EGuides, FLanguageTextures> Textures;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* MapInfoOpenAnimation;

public:
	void PlayMapInfoAnimation(EGuides MapName, bool isOpen = true);
};
