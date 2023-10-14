// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include <SERVAME/Manager/JesusGameInstance.h>
#include "PlayerGuideUI.generated.h"

#define Gamepad 0
#define Keyboard 1

/**
 * 
 */
UENUM()
enum class EGuides : uint8
{
	keyguide	UMETA(DisplayName = "KeyGuide"),
	dodge		UMETA(DisplayName = "Dodge"),
	camera		UMETA(DisplayName = "Camera"),
	grogy		UMETA(DisplayName = "Grogy"),
	parrying	UMETA(DisplayName = "Parrying"),
	savepoint	UMETA(DisplayName = "SavePoint"),
	shield			UMETA(DisplayName = "Shield"),
	shieldattack			UMETA(DisplayName = "Shieldattack"),
	soul			UMETA(DisplayName = "Soul"),
	
	LockOn		UMETA(DisplayName = "LockOn"),
	Parring		UMETA(DisplayName = "Parring"),
	Heal		UMETA(DisplayName = "Heal"),
	ShieldBash	UMETA(DisplayName = "ShieldBash"),
	Shield		UMETA(DisplayName = "Shield"),
	StrongAttack	UMETA(DisplayName = "StrongAttack"),

	Haros	UMETA(DisplayName = "Haros"),
	LostWay UMETA(DisplayName = "LostWay"),
	HouseOfAbandoned	UMETA(DisplayName = "HouseOfAbandoned"),
	MainHall	UMETA(DisplayName = "MainHall"),
	Repent	UMETA(DisplayName = "Repent")
};

USTRUCT(BlueprintType)
struct SERVAME_API FGuideTextures
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> KorTextures;
	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> EngTextures;

	bool isRenderedOnce = false;
};

UCLASS()
class SERVAME_API UPlayerGuideUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* GuideImage;

	UPROPERTY(EditAnywhere)
	TMap<EGuides, FGuideTextures> GuideTextures;

	int isGamePad = Keyboard;
	bool IsRenderImage;
public:
	void SetGamePad(int isPad);
	void StartGuide(EGuides type, Language language);

	bool IsRender();

	void Disable();
};
