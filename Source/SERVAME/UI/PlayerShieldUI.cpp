// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShieldUI.h"

void UPlayerShieldUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Shields.Add(ShieldUI_0);
	Shields.Add(ShieldUI_1);
	Shields.Add(ShieldUI_2);
}

void UPlayerShieldUI::SetShield(int value)
{
	value = FMath::Clamp(value, 0, Shields.Num() - 1);
	int i = 0;
	for (; i < value + 1; i++)
	{
		Shields[i]->Activate();
		//if (i == Shields.Num() - 1)
		//	Shields[i]->ActivateFire();
	}
	for (; i < Shields.Num(); i++)
	{
		Shields[i]->Deactivate();
	}

	if (value == Shields.Num() - 1)
	{
		for (i = 0; i < Shields.Num(); i++)
		{
			Shields[i]->ActivateFire();
		}
	}
}

void UPlayerShieldUI::Clear()
{
	for (int i = 0; i < Shields.Num(); i++)

	{
		Shields[i]->Deactivate();
	}
}
