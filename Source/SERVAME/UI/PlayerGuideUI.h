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
	Skill		UMETA(DisplayName = "Tutorial_Skill"),
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
	ST_Garden	UMETA(DisplayName = "ST_Garden"),
	ST_MainHall	UMETA(DisplayName = "ST_MainHall"),
	ST_BossHall	UMETA(DisplayName = "ST_BossHall"),
	ST_Tomb		UMETA(DisplayName = "ST_Tomb"),
	ST_PrayRoom	UMETA(DisplayName = "ST_PrayRoom"),
	ST_Jamsic	UMETA(DisplayName = "ST_Jamsic"),
	ST_Elgi		UMETA(DisplayName = "ST_Elgi"),
	ST_Nun1		UMETA(DisplayName = "ST_Nun_1"),
	ST_Nun3		UMETA(DisplayName = "ST_Nun_3"),
	ST_Phase1_1	UMETA(DisplayName = "ST_Phase1_1"),
	ST_Phase1_3	UMETA(DisplayName = "ST_Phase1_3"),
	ST_Phase1_4	UMETA(DisplayName = "ST_Phase1_4"),
	ST_Phase2_1	UMETA(DisplayName = "ST_Phase2_1"),
	ST_Phase2_2	UMETA(DisplayName = "ST_Phase2_2"),

	// 무슨 이상한 가이드
	SoulGuide	UMETA(DisplayName = "SoulGuide"),
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



	int isGamePad = Keyboard;
	bool IsRenderImage;
public:
	UPROPERTY(meta = (BindWidget))
	UImage* GuideImage;
	UPROPERTY(EditAnywhere)
	TMap<EGuides, FGuideTextures> GuideTextures;
	void SetGamePad(int isPad);
	void StartGuide(EGuides type, Language language);

	bool IsRender();

	void Disable();
};
