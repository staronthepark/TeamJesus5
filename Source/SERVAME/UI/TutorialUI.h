// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerGuideUI.h"
#include "TutorialUI.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct SERVAME_API FTutorialTextures
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		UTexture2D* KorPc;

	UPROPERTY(EditAnywhere)
		UTexture2D* EngPc;

	UPROPERTY(EditAnywhere)
		UTexture2D* KorPad;

	UPROPERTY(EditAnywhere)
		UTexture2D* EngPad;
};

UCLASS()
class SERVAME_API UTutorialUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<EGuides, FTutorialTextures> textures;

	UPROPERTY(meta = (BindWidget))
	class UImage* TutorialImage;

public:
	void ChangeTexture(EGuides type, int isPad);
};
