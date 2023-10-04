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

enum class Language : uint8;

UENUM()
enum class EHoverTextures : uint8
{
	hovered		UMETA(DiplayName = "Hovered"),
	unhovered	UMETA(DisplayName = "Unhovered")
};

USTRUCT(BlueprintType)
struct SERVAME_API FSelectTexture
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		UTexture2D* KorTextures;
	UPROPERTY(EditAnywhere)
		UTexture2D* EngTextures;

};
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
		TMap<EHoverTextures, FSelectTexture> SelectTextTextures;
public:
	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void Select();

	UFUNCTION()
	void Unselect();

	UFUNCTION()
	void OnHovered();

	UFUNCTION()
	void OnUnhovered();

	void ChangeLanguage(Language& language);
};
