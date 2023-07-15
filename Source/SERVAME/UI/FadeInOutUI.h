// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeInOutUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UFadeInOutUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* FadeInAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* FadeOutAnimation;

public:
	void PlayFadeInOutAnimation(bool isFadeIn);
};
