// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "../Player/PlayerStatComponent.h"

#include "StatUI.generated.h"

/**
 * 
 */

UENUM()
enum class EButtonState : uint8
{
	normal		UMETA(DisplayName = "Normal"),
	hovered		UMETA(DisplayName = "Hovered"),
	pressed		UMETA(DisplayName = "Pressed")
};

USTRUCT(BlueprintType)
struct SERVAME_API FStatButtonTexture
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		UTexture2D* Texture;
};

UENUM()
enum class EStatState : uint8
{
	closed		UMETA(DisplayName = "Closed"),
	can			UMETA(DisplayName = "CanActivate"),
	activated	UMETA(DisplayName = "Activated")
};

UENUM()
enum class EStateType : uint8
{
	str		UMETA(DisplayName = "Strength"),
	stamina	UMETA(DisplayName = "Stamina"),
	hp		UMETA(DiaplayName = "Hp"),
	shield	UMETA(DisplayName = "Shield")
};

UCLASS()
class SERVAME_API UStatUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<EButtonState, FStatButtonTexture> ButtonStates;

	UPROPERTY(EditAnywhere)
	TArray<UImage*> Lines;

	UPROPERTY(EditAnywhere)
	UTexture2D* LineTexture;

	UPROPERTY(EditAnywhere)
	UTexture2D* ExplainTitle;	
	
	UPROPERTY(EditAnywhere)
	UTexture2D* ExplainIcon;	
	
	UPROPERTY(EditAnywhere)
	UTexture2D* ExplainText;


	UPROPERTY(EditAnywhere)
	UStatUI* NextStat;

	UPROPERTY(EditAnywhere)
		float x;
	UPROPERTY(EditAnywhere)
		float y;

	UPROPERTY(EditAnywhere)
	int Cost;

	UPROPERTY(EditAnywhere)
	int Value;

	UPROPERTY(meta = (BindWidget))
	UImage* ActiveBackground;

	UPROPERTY(EditAnywhere)
	EStateType Type;

	TMap<EStateType, TFunction<bool(int32 index)>> TypeAnimation;

	UPlayerStatComponent* PlayerStatComp;
	
public:

	class UPlayerStatUI* ParentUI;

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	UPROPERTY(EditAnywhere)
	EStatState state;

	int index;

	virtual void NativeOnInitialized();
	void ChangeState(EStatState changeState);
	EStatState GetState();

	void Init();

	UFUNCTION()
	void OnButtonHovered();

	UFUNCTION()
	void OnButtonUnhovered();

	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION()
	void OnButtonUnclicked();

	UFUNCTION()
	void Activate();


	UFUNCTION()
	void ChangeButtonToActivate();
};