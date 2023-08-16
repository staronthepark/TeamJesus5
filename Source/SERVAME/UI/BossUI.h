// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include <Components/Slider.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include "MediaPlayer.h"

#include "BossUI.generated.h"

/**
 * 
 */

UENUM()
enum class EBossSettings :uint8
{
	phase1	UMETA(DisplayName = "Phase1"),
	phase2	UMETA(DisplayName = "Phase2")
};

USTRUCT(BlueprintType)
struct SERVAME_API FBossNameTextures
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere)
		UTexture2D* KorTexture;
	UPROPERTY(EditAnywhere)
		UTexture2D* EngTexture;
};

UCLASS()
class SERVAME_API UBossUI : public UBaseUI
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* BossNameText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Boss_HP_Y;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Boss_HP;
	UPROPERTY(meta = (BindWidget))
	USlider* Boss_White_P;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TakeDamageText;

	UPROPERTY(EditAnywhere)
	TMap<EBossSettings, FBossNameTextures> BossNameTextures;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BossHPOpenAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeInAnimation;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOutAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* GameClearAnimation;

	FTimerHandle HpDelayTimerHandle;
	FTimerHandle HpProgressTimerHandle;

	FTimerHandle DamageTimerHandle;
	float CumulateDamage;

	//MediaPlayer
	//UPROPERTY(meta = (BindWidget))
	//	UImage* EndingCreditImage;
	//UPROPERTY(EditAnywhere)
	//	UMediaPlayer* MediaPlayer;
	//UPROPERTY(EditAnywhere)
	//	UMediaSource* MediaSource;

public:
	FWidgetAnimationDynamicEvent EndDelegate;
	FWidgetAnimationDynamicEvent GameClearDelegate;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	void SetHP(float value);
	void DecreaseHPGradual(AActor* Actor, float value);
	void PlayBossHPOpenAnimation(bool IsOpen);

	void PlayBossDiedAnimtion();

	void PlayFadeInAnimation();

	void PlayFadeOutAnimation();

	void PlayGameClearAnimation();

	void SetDamageText(float value);

	UFUNCTION()
		void OnAnimationEnd();

	UFUNCTION()
		void ChangeLanguage();
};
