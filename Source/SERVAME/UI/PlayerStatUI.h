// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include "PlayerStatUI.generated.h"

/**
 * 
 */

class UStatUI;
class UImage;

UENUM()
enum class EStatsType : uint8
{
	str		UMETA(DisplayName = "Strength"),
	stamina	UMETA(DisplayName = "Stamina"),
	hp		UMETA(DiaplayName = "Hp"),
	shield	UMETA(DisplayName = "Shield")
};

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

	UPROPERTY(meta = (BindWidget))
	UStatUI* StrButton4;
	

	TArray<UStatUI*> StaminaButtons;

	UPROPERTY(meta = (BindWidget))
		UStatUI* StaminaButton0;

	UPROPERTY(meta = (BindWidget))
		UStatUI* StaminaButton1;

	UPROPERTY(meta = (BindWidget))
		UStatUI* StaminaButton2;

	UPROPERTY(meta = (BindWidget))
		UStatUI* StaminaButton3;

	UPROPERTY(meta = (BindWidget))
		UStatUI* StaminaButton4;

	TArray<UStatUI*> HpButtons;

	UPROPERTY(meta = (BindWidget))
		UStatUI* HpButton0;

	UPROPERTY(meta = (BindWidget))
		UStatUI* HpButton1;

	UPROPERTY(meta = (BindWidget))
		UStatUI* HpButton2;

	UPROPERTY(meta = (BindWidget))
		UStatUI* HpButton3;

	UPROPERTY(meta = (BindWidget))
		UStatUI* HpButton4;

	TArray<UStatUI*> ShieldButtons;

	UPROPERTY(meta = (BindWidget))
		UStatUI* ShieldButton0;

	UPROPERTY(meta = (BindWidget))
		UStatUI* ShieldButton1;

	UPROPERTY(meta = (BindWidget))
		UStatUI* ShieldButton2;

	UPROPERTY(meta = (BindWidget))
		UStatUI* ShieldButton3;

	UPROPERTY(meta = (BindWidget))
		UStatUI* ShieldButton4;

	UPROPERTY(EditAnywhere)
	TMap<EStatsType, FStatButtonExplainTextures> ExplainTextures;

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

	virtual void NativeConstruct();

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

	// 이게 남은 영혼!!
	void SetRemainSoul(int value);

	void Open();

	void Close();

	/*virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);*/
};
