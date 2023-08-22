// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include "SheldUI.generated.h"

/**
 * 
 */


UCLASS()
class SERVAME_API USheldUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* ShieldImage;

	UPROPERTY(meta = (BindWIdget))
		UImage* ActivateUI;

	UPROPERTY(EditAnywhere)
		TArray<UTexture2D*> ShieldImages;

public:
	void Activate();
	void Deactivate();

	void ActivateFire();
};
