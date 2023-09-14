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
	EStatState state;

	UPROPERTY(EditAnywhere)
	UStatUI* NextStat;

	UPROPERTY(meta = (BindWidget))
	UImage* ActiveBackground;

	UPROPERTY(EditAnywhere)
	EStateType Type;

	TMap<EStateType, TFunction<void(int32 index)>> TypeAnimation;

	UPlayerStatComponent* PlayerStatComp;
	
public:

	class UPlayerStatUI* ParentUI;

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	int index;

	virtual void NativeOnInitialized();
	void ChangeState(EStatState changeState);


	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION()
	void OnButtonUnclicked();

	UFUNCTION()
	void Activate();

};