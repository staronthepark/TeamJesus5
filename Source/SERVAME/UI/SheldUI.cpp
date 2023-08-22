
// Fill out your copyright notice in the Description page of Project Settings.


#include "SheldUI.h"

#define DEACTIVE 0
#define ACTIVE 1

void USheldUI::Activate()
{
	ShieldImage->SetBrushFromTexture(ShieldImages[ACTIVE]);
}

void USheldUI::Deactivate()
{
	ShieldImage->SetBrushFromTexture(ShieldImages[DEACTIVE]);
	ActivateUI->SetVisibility(ESlateVisibility::Collapsed);
}

void USheldUI::ActivateFire()
{
	ActivateUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
