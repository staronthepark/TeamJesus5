// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SheldUI.h"
#include "PlayerShieldUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UPlayerShieldUI : public UUserWidget
{
	GENERATED_BODY()
	TArray<USheldUI*> Shields;

	UPROPERTY(meta = (BindWidget))
	USheldUI* ShieldUI_0;

	UPROPERTY(meta = (BindWidget))
	USheldUI* ShieldUI_1;

	UPROPERTY(meta = (BindWidget))
	USheldUI* ShieldUI_2;

public:
	virtual void NativeOnInitialized();

	void SetShield(int value);
};
