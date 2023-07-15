// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillIconUI.h"

void USkillIconUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//index = 0;
	//Skill_Image->SetBrushFromTexture(SkillIcons[index]);
}

void USkillIconUI::ChangeSkill(const bool isPressedRight)
{
	//if (isPressedRight)
	//	Skill_Image->SetBrushFromTexture(SkillIcons[FMath::Abs(++index % SkillIcons.Num())]);
	//else
	//	Skill_Image->SetBrushFromTexture(SkillIcons[FMath::Abs(--index % SkillIcons.Num())]);
	//
}

void USkillIconUI::ChangeHealCount(int value)
{
	HealCountText->SetText(FText::AsNumber(value));
}
