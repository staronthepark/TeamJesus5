// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapUI.generated.h"

/**
 * 
 */

UCLASS()
class SERVAME_API UMapUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* MapInfoOpenAnimation;

public:
	void PlayMapInfoAnimation(FString MapName, bool isOpen = true);
};
