// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/ProgressBar.h>
#include "GameFramework/Actor.h"
#include "BaseUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBaseUI : public UUserWidget
{
	GENERATED_BODY()

	UProgressBar* TempProgressBar;
	
	float min;
	float max;

protected:
	void ProgressDecrease(AActor* Actor, UProgressBar* ProgressBar, float _min, float _max, FTimerHandle& DelayTimerHandle, FTimerHandle& ProgressTimerHandle);
};
