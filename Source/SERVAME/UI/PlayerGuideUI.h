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
	//shield			UMETA(DisplayName = "Shield"),
	shieldattack			UMETA(DisplayName = "Shieldattack"),
	soul			UMETA(DisplayName = "Soul"),

	
	// Tutorial
	LockOn		UMETA(DisplayName = "Tutorial_LockOn"),
	Parring		UMETA(DisplayName = "Tutorial_Parring"),
	Heal		UMETA(DisplayName = "Tutorial_Heal"),
	ShieldBash	UMETA(DisplayName = "Tutorial_ShieldBash"),
	Shield		UMETA(DisplayName = "Tutorial_Shield"),
	StrongAttack	UMETA(DisplayName = "Tutorial_StrongAttack"),

	// Maps
	Haros	UMETA(DisplayName = "Map_Haros"),
	LostWay UMETA(DisplayName = "Map_LostWay"),
	HouseOfAbandoned	UMETA(DisplayName = "Map_HouseOfAbandoned"),
	MainHall	UMETA(DisplayName = "Map_MainHall"),
	Repent	UMETA(DisplayName = "Map_Repent"),

	// Subtitles
	BossRoomWay UMETA(DisplayName = "ST_BossRoomWay"),
	EliteKnight	UMETA(DisplayName = "ST_EliteKnight"),
	Garden		UMETA(DisplayName = "ST_Garden"),
	JotbabMonster	UMETA(DisplayName = "ST_Jotbab"),
	Nun			UMETA(DisplayName = "ST_Nun"),
	Door		UMETA(DisplayName = "ST_Door"),
	PrayRoom	UMETA(DisplayName = "ST_PrayRoom"),
	Tomb		UMETA(DisplayName = "ST_Tomb"),
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
