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

enum class Language :uint8;

USTRUCT(BlueprintType)
struct SERVAME_API FButtonTextures
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		UTexture2D* KorTexture;
	UPROPERTY(EditAnywhere)
		UTexture2D* EngTexture;
};

UCLASS()
class SERVAME_API UButtonUI : public UUserWidget
{
	GENERATED_BODY()

	int index; 

	UPROPERTY(meta = (BindWidget))
		UImage* OnOffImage;

	//UPROPERTY(EditAnywhere)
	//	TArray<UTexture2D*> OnOffImages;

	UPROPERTY(EditAnywhere)
		TMap<int, FButtonTextures> OnOffImages;

	UPROPERTY(EditAnywhere)
		TArray<UTexture2D*> LeftButtonHoveredImages;

	UPROPERTY(EditAnywhere)
		TArray<UTexture2D*> RightButtonHoveredImages;

public:
	UPROPERTY(meta = (BindWidget))
		UButton* LeftButton;
	UPROPERTY(meta = (BindWidget))
		UButton* RightButton;

	TArray<UButton*> Buttons;
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void LeftButtonClicked();
	UFUNCTION()
	void RightButtonClicked();

	int GetValue();
	void SetValue(int value);

	UFUNCTION()
	void LeftButtonHovered();

	UFUNCTION()
	void RightButtonHovered();

	UFUNCTION()
	void LeftButtonUnhovered();

	UFUNCTION()
	void RightButtonUnhovered();

	void ChangeLanguage(Language& language);
};
