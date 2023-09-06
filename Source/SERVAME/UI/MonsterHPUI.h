// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include <Components/Slider.h>
#include "MonsterHPUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UMonsterHPUI : public UBaseUI
{
	GENERATED_BODY()
	
		UPROPERTY(meta = (BindWidget))
		UProgressBar* Monster_HP_Bar;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* Monster_HP_Bar_Y;


	UPROPERTY(meta = (BindWidget))
		USlider* HP_White_P;

	FTimerHandle HpDelayTimerHandle;
	FTimerHandle HpProgressTimerHandle;

public:
	void SetHP(float value);
	void DecreaseHPGradual(AActor* Actor, float value);
};
