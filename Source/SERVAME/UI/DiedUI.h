// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Border.h>
#include <Components/Image.h>
#include "DiedUI.generated.h"

/**
* 
 */

UCLASS()
class SERVAME_API UDiedUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* DiedInAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* DiedOutAnimation;

	UFUNCTION()
	void DiedFadeOutAnimation();
	FWidgetAnimationDynamicEvent DiedInAnimationEvent;

public:
	virtual void NativeOnInitialized() override;
	void PlayDiedAnimation(bool IsOpen);
};
