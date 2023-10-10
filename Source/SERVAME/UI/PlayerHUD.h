// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define Gamepad 0
#define Keyboard 1

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Class.h"
#include "DiedUI.h"
#include "MapUI.h"
#include "FadeInOutUI.h"
#include "PlayerHPUI.h"
#include "SkillIconUI.h"
#include "PlayerGuideUI.h"
#include "PlayerShieldUI.h"
#include <Components/CanvasPanel.h>
#include "PlayerHUD.generated.h"


/**
 * 
 */

class USkillUI;

UENUM()
enum class EInteractions : uint8
{
	interaction UMETA(DisplayName = "Interaction"),
	close		UMETA(DisplayName = "Close"),
	talk		UMETA(DisplayName = "Talk"),
	open		UMETA(DisplayName = "Open"),
	cure		UMETA(DisplayName = "Cure"),
	rest		UMETA(DisplayName = "Rest")
};

UENUM(BlueprintType)
enum class EPlayerStatType : uint8
{
	STRENGTH UMETA(DisplayName = "Strength"),
	HP		UMETA(DisplayName = "HP"),
	STAMINA	UMETA(DisplayName = "STAMINA"),
};

USTRUCT(BlueprintType)
struct SERVAME_API FInteractionTextures 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> KorTextures;
	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> EngTextures;
};

UCLASS()
class SERVAME_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UDiedUI* WBP_DiedUI;

	UPROPERTY(meta = (BindWidget))
		UMapUI* WBP_MapInfo;

	UPROPERTY(meta = (BindWidget))
		UPlayerHPUI* WBP_PlayerHPUI;
	
	UPROPERTY(meta = (BindWidget))
		UImage* PlayerControlGuid;

	UPROPERTY(meta = (BindWidget))
		UImage* Interaction_Image;

	UPROPERTY(meta = (BindWidget))
		USkillUI* SkillUI;

	//UPROPERTY(meta = (BindWidget))
	//	UPlayerShieldUI* WBP_Player_ShieldsUI;

	//UPROPERTY(meta = (BindWidget))
	//	UImage* Exit_Image;
	
	UPROPERTY(meta = (BindWidget))
		UFadeInOutUI* WBP_FadeInOutUI;

	UPROPERTY(meta = (BindWidget))
		USkillIconUI* WBP_SkillIconUI;

	UPROPERTY(meta = (BindWidget))
		UPlayerGuideUI* WBP_PlayerGuideUI;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* HUDOpenAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* GuidOpenAnimation;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* GuidCloseAnimation;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* InteractionOpacityAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* InteractionOpacityOutAnimation;

	//UPROPERTY(Transient, meta = (BindWidgetAnim))
	//	UWidgetAnimation* ExitOpenAnimation;

	//UPROPERTY(Transient, meta = (BindWidgetAnim))
	//	UWidgetAnimation* ExitCloseAnimation;

	UPROPERTY(EditAnywhere)
	TMap<EInteractions, FInteractionTextures> InteractionTextures;

	//	TMap<int, TFunction<void(bool)>> TriggerMap;

	UPROPERTY(EditAnywhere)
	TMap<int, FInteractionTextures> ControlGuideTextures;

	TMap <EGuides, TFunction<void(bool)>> GuideAnimationFunction;



	int isGamePad;
	Language currentLanguage;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	void SetHP(float value);
	void DecreaseHPGradual(AActor* Actor, float value);

	void SetStamina(float value);
	void DecreaseStaminaGradual(AActor* Actor, float value);

	void SetSoul(float value);
	void DecreaseSoulGradual(AActor* Actor, float value);

	void SetSkill(float value);
	
	void SetGamepad(bool isPad);

	void PlayAnimations(EGuides type, bool IsOpen);
	//void SetShield(int value);

	void PlayGuidesAnimation(EGuides type ,bool IsOpen);
	void PlayKeyGuideAnimation(bool IsOpen);
	void PlayInteractionAnimation(bool IsOpen, EInteractions interactions);
	void PlayInteractionAnimation(bool IsOpen);
	void PlayExitAnimation(bool IsOpen);
	void PlayMapUIAnimation(EMapName MapName);

	void FadeInAnimation(bool isFadeIn);
	void ClearShield();

	bool IsRender();

	void PlayDied(bool IsOpen);
	void ChangeSkill(const bool isPressedRight);
	void ChangeHealCount(int value);
};

