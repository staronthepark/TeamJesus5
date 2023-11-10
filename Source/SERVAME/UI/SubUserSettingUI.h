// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubUserSettingUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API USubUserSettingUI : public UUserWidget
{
	GENERATED_BODY()

public:
	
	int NavigationIndex;
	virtual void NativeOnInitialized() override;
};
