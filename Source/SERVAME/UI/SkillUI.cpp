// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillUI.h"
#include <Components/Image.h>

void USkillUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Material = Background->GetDynamicMaterial();
}

void USkillUI::SetSkill(float value)
{
	Material->SetScalarParameterValue(TEXT("Value"), value);
}


