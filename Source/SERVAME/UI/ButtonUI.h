// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/Image.h>

#include "ButtonUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UButtonUI : public UUserWidget
{
	GENERATED_BODY()


	UPROPERTY(meta = (BindWidget))
		UImage* OnOffImage;

	UPROPERTY(EditAnywhere)
		TArray<UTexture2D*> OnOffImages;

	int index; 

public:
	UPROPERTY(meta = (BindWidget))
		UButton* LeftButton;
	UPROPERTY(meta = (BindWidget))
		UButton* RightButton;
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void LeftButtonClicked();
	UFUNCTION()
	void RightButtonClicked();

	int GetValue();
	void SetValue(int value);
};
