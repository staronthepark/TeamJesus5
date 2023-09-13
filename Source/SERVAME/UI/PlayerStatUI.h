// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include "StatUI.h"
#include "PlayerStatUI.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SERVAME_API FStatButtonExplainTextures
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		UTexture2D* TitleTexture;

	UPROPERTY(EditAnywhere)
		UTexture2D* IconTexture;

	UPROPERTY(EditAnywhere)
		UTexture2D* ExplainText;
};

UCLASS()
class SERVAME_API UPlayerStatUI : public UUserWidget
{
	GENERATED_BODY()

	TArray<UStatUI*> StrButtons;

	UPROPERTY(meta = (BindWidget))
	UStatUI* StrButton0;
	
	UPROPERTY(meta = (BindWidget))
	UStatUI* StrButton1;
	
	UPROPERTY(meta = (BindWidget))
	UStatUI* StrButton2;
	
	UPROPERTY(meta = (BindWidget))
	UStatUI* StrButton3;
	

	UPROPERTY(EditAnywhere)
	TArray<UStatUI*> StaminaButtons;

	UPROPERTY(EditAnywhere)
	TArray<UStatUI*> HpButtons;

	UPROPERTY(EditAnywhere)
	TArray<UStatUI*> ShieldButtons;

	UPROPERTY(EditAnywhere)
	TMap<EStateType, FStatButtonExplainTextures> ExplainTextures;

	UPROPERTY(meta = (BindWidget))
	UImage* ExplainTitle;

	UPROPERTY(meta = (BindWidget))
	UImage* ExplainIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* ExplainText;


	UPROPERTY(meta = (BindWidget))
	UButton* PurchaseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ResetButton;

	UPROPERTY(meta = (BindiWidget))
	UTextBlock* RemainSoulText;

public:
	UStatUI* SelectedButton;

	virtual void NativeOnInitialized();

	UFUNCTION()
	void OnStrButtonClicked();

	UFUNCTION()
	void OnStaminaButtonClicked();

	UFUNCTION()
	void OnHpButtonClicked();

	UFUNCTION()
	void OnShieldButtonClicked();

	UFUNCTION()
	void OnPurchaseButtonClicked();
};
