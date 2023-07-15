// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHPUI.h"

void UPlayerHPUI::SetHP(float value)
{
	Player_HP_Bar->SetValue(value);
}

void UPlayerHPUI::DecreaseHPGradual(AActor* Actor, float value)
{	
	Player_HP_Bar->DecreaseGradual(Actor, value);
}

void UPlayerHPUI::SetStamina(float value)
{
	Player_Stamina_Bar->SetValue(value);
}

void UPlayerHPUI::DecreaseStaminaGradual(AActor* Actor, float value)
{
	Player_Stamina_Bar->DecreaseGradual(Actor, value);
}

//void UPlayerHPUI::SetSkill(float value)
//{
//	value = FMath::Clamp(value, 0.0f, 1.0f);
//	int index = (int)(value * (SkillImage.Num() - 1));
//	UE_LOG(LogTemp, Warning, TEXT("%d"), index);
//	Player_Skill_Image->SetBrushFromTexture(SkillImage[index]);
//}
