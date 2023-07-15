// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "SkillIconUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API USkillIconUI : public UUserWidget
{
	GENERATED_BODY()

	//UPROPERTY(meta = (BindWidget))
	//	UImage* Skill_Image;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* HealCountText;

	//UPROPERTY(EditAnywhere)
	//	TArray<UTexture2D*> SkillIcons;

	//int index;

public:
	virtual void NativeOnInitialized() override;

	void ChangeSkill(const bool isPressedRight);
	void ChangeHealCount(int value);
};
