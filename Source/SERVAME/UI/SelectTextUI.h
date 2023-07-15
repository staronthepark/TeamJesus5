// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include <Components/Button.h>

#include "SelectTextUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API USelectTextUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* SelectedBackgroundImage;		
	UPROPERTY(meta = (BindWidget))
		UImage* LeftButton;		
	UPROPERTY(meta = (BindWidget))
		UImage* RightButton;		
	UPROPERTY(meta = (BindWidget))
		UImage* Text;


	UPROPERTY(EditAnywhere)
		TArray<UTexture2D*> TextIcons;

public:
	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void Select();

	UFUNCTION()
	void Unselect();

	UFUNCTION()
	void OnHovered();

	UFUNCTION()
	void OnUnhovered();
};
