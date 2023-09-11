// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHPUI.h"

void UMonsterHPUI::SetHP(float value)
{
	value = FMath::Clamp(value, 0.0f, 1.0f);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	Monster_HP_Bar->SetPercent(value);
	HP_White_P->SetValue(value);
	if (!TimerManager.IsTimerActive(HpDelayTimerHandle) && !TimerManager.IsTimerActive(HpProgressTimerHandle))
		Monster_HP_Bar_Y->SetPercent(value);
}

void UMonsterHPUI::DecreaseHPGradual(AActor* Actor, float value)
{
	value = FMath::Clamp(value, 0.0f, 1.0f);

	Monster_HP_Bar->SetPercent(value);
	HP_White_P->SetValue(value);
	ProgressDecrease(Actor, Monster_HP_Bar_Y, value, 1, HpDelayTimerHandle, HpProgressTimerHandle);
}
