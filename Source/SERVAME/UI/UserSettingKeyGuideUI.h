// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserSettingKeyGuideUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UUserSettingKeyGuideUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* KeyboardImage;
	
	UPROPERTY(EditAnywhere)
	UTexture2D* PadImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* KeyGuideImage;

};
