// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillUI.generated.h"


/**
 * 
 */
UCLASS()
class SERVAME_API USkillUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UImage* Background;

	UMaterialInstanceDynamic* Material;

	FTimerHandle TimerHandle;

public:
	virtual void NativeOnInitialized() override;

	void SetSkill(float seconds);
};
