
// Fill out your copyright notice in the Description page of Project Settings.


#include "SheldUI.h"

#define DEACTIVE 0
#define ACTIVE 1

void USheldUI::Activate()
{
	ShieldImage->SetBrushFromTexture(ShieldImages[ACTIVE]);
	ActiveEffect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	EffectScalarValue = 0;
	TimerManager.SetTimer(EffectTimerHandle, [&]() {
		EffectScalarValue += 0.01f;
		if (EffectScalarValue >= 1.0f)
			TimerManager.ClearTimer(EffectTimerHandle);
		ActiveEffect->GetDynamicMaterial()->SetScalarParameterValue("Dissolve", EffectScalarValue);
		ActiveEffect->GetDynamicMaterial()->SetScalarParameterValue("Offset", EffectScalarValue);
		ActiveEffect->GetDynamicMaterial()->SetScalarParameterValue("Tilling", EffectScalarValue);
		}, 0.01f, true);
}

void USheldUI::Deactivate()
{
	ShieldImage->SetBrushFromTexture(ShieldImages[DEACTIVE]);
	ActivateUI->SetVisibility(ESlateVisibility::Collapsed);
	ActiveEffect->SetVisibility(ESlateVisibility::Collapsed);
}

void USheldUI::ActivateFire()
{
	ActivateUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
