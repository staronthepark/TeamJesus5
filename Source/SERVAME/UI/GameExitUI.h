// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include "GameExitUI.generated.h"

/**
 * 
 */

enum class Language :uint8;

UENUM()
enum class EExitSettings : uint8
{
	title		UMETA(DisplayName = "Title"),
	yesnormal	UMETA(DisplayName = "YesNormal"),
	yeshovered	UMETA(DisplayName = "YesHovered"),
	yespressed	UMETA(DisplayName = "YesPressed"),
	nonormal	UMETA(DisplayName = "NoNormal"),
	nohovered	UMETA(DisplayName = "NoHovered"),
	nopressed	UMETA(DisplayName = "NoPressed")
};

USTRUCT(BlueprintType)
struct SERVAME_API FExitTextures
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		UTexture2D* KorTexture;
	UPROPERTY(EditAnywhere)
		UTexture2D* EngTexture;
};

UCLASS()
class SERVAME_API UGameExitUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UImage* PanelBackground;

	UPROPERTY(EditAnywhere)
	TMap<EExitSettings, FExitTextures> ImageTextures;

	TArray<UButton*> Buttons;
	int index;

public:
	UPROPERTY(meta = (BindWidget))
	UButton* YesButton;
	UPROPERTY(meta = (BindWidget))
	UButton* NoButton;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void Open();
	
	UFUNCTION()
	void Close();

	UFUNCTION()
	void YesButtonClicked();
	UFUNCTION()
	void NoButtonClicked();

	UFUNCTION()
	void YesButtonHovered();

	UFUNCTION()
	void NoButtonHovered();

	UFUNCTION()
	void YesButtonUnhovered();

	UFUNCTION()
	void NoButtonUnhovered();

	UFUNCTION()
	void ChangeLanguage(Language& language);

	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
