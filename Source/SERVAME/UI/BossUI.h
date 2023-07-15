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
UCLASS()
class SERVAME_API UBossUI : public UBaseUI
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Boss_HP_Y;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Boss_HP;
	UPROPERTY(meta = (BindWidget))
	USlider* Boss_White_P;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TakeDamageText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BossHPOpenAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BossDiedAnimation;

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
	FWidgetAnimationDynamicEvent EndDelegate;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	void SetHP(float value);
	void DecreaseHPGradual(AActor* Actor, float value);
	void PlayBossHPOpenAnimation(bool IsOpen);

	void PlayBossDiedAnimtion();

	void SetDamageText(float value);

	UFUNCTION()
		void OnAnimationEnd();
};
