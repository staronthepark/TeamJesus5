// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "MonsterWidget.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UMonsterWidget : public UUserWidget
{
	GENERATED_BODY()


	void Init();

public:
	UPROPERTY(meta = (BindWidget))
		UImage* LockOnImage;
};