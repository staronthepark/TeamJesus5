// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>

#include "GameExitUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UGameExitUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UButton* YesButton;
	UPROPERTY(meta = (BindWidget))
		UButton* NoButton;

public:
	virtual void NativeOnInitialized() override;
	UFUNCTION()
	void YesButtonClicked();
	UFUNCTION()
	void NoButtonClicked();
};
