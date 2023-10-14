// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillUI.h"
#include <Components/Image.h>

void USkillUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Material = Background->GetDynamicMaterial();
	value = 1;
}

void USkillUI::SetSkill(float seconds)
{
	value = 0;
	Material->SetScalarParameterValue(TEXT("Value"), 0);
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle, [&]() {
		value += 0.1f;
		Material->SetScalarParameterValue(TEXT("Value"), value);
		if (value >= 1) {
			value = 1;
			Material->SetScalarParameterValue(TEXT("Value"), value);
			TimerManager.ClearTimer(TimerHandle);
		}
		}, 1/seconds , true);
}


