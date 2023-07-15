// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUI.h"
#include <Components/Slider.h>
#include <Components/Image.h>
#include "ProgressiveBarUI.h"
#include "PlayerHPUI.generated.h"
/**
 * 
 */
UCLASS()
class SERVAME_API UPlayerHPUI : public UBaseUI
{
	GENERATED_BODY()

		//UPROPERTY(meta = (BindWidget))
		//	UProgressBar* Player_HP_Bar;

		//UPROPERTY(meta = (BindWidget))
		//	UProgressBar* Player_HP_Bar_Y;


		//UPROPERTY(meta = (BindWidget))
		//	USlider* HP_White_P;	

		//UPROPERTY(meta = (BindWidget))
		//	UProgressBar* Player_Stamina_Bar;

		//UPROPERTY(meta = (BindWidget))
		//	UProgressBar* Player_Stamina_Bar_Y;

		//UPROPERTY(meta = (BindWidget))
		//	USlider* Stamina_White_P;

	UPROPERTY(meta = (BindWidget))
		UProgressiveBarUI* Player_HP_Bar;
	UPROPERTY(meta = (BindWidget))
		UProgressiveBarUI* Player_Stamina_Bar;

	//UPROPERTY(meta = (BindWidget))
	//	UImage* Player_Skill_Image;

	//UPROPERTY(EditAnywhere)
	//	TArray<UTexture2D*> SkillImage;

	//FTimerHandle HpDelayTimerHandle;
	//FTimerHandle HpProgressTimerHandle;

	//FTimerHandle StaminaDelayTimerHandle;
	//FTimerHandle StaminaProgressTimerHandle;

public:
	void SetHP(float value);
	void DecreaseHPGradual(AActor* Actor, float value);

	void SetStamina(float value);
	void DecreaseStaminaGradual(AActor* Actor, float value);

	//void SetSkill(float value);
};
