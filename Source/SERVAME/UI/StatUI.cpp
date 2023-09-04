// Fill out your copyright notice in the Description page of Project Settings.

#include "StatUI.h"

void UStatUI::NativeOnInitialized()
{
	PlayerStatComp = Cast<UPlayerStatComponent>(GetWorld()->GetFirstPlayerController()->GetOwner()->GetComponentByClass(UPlayerStatComponent::StaticClass()));
	
	Button->OnClicked.AddDynamic(this, &UStatUI::OnButtonClicked);
	TypeAnimation.Add(EStateType::str, [&](int32 index) {
		PlayerStatComp->StrengthStatList[index].Func();
		});
	TypeAnimation.Add(EStateType::stamina, [&](int32 index) {
		PlayerStatComp->StaminaStatList[index].Func();

		});
	TypeAnimation.Add(EStateType::hp, [&](int32 index) {
		PlayerStatComp->HpStatList[index].Func();

		});
	TypeAnimation.Add(EStateType::shield, [&](int32 index) {
		PlayerStatComp->ShieldStatList[index].Func();
		});
}

void UStatUI::ChangeState(EStatState changeState)
{
	state = changeState;
}

void UStatUI::OnButtonClicked()
{
	if (state == EStatState::closed || state == EStatState::activated)
		return;

	if(state == EStatState::can)
	{
		state = EStatState::activated;

		int32 index = 0;

		TypeAnimation[Type](index);

		for (int i = 0; i < NextStat.Num(); i++)
		{
			NextStat[i]->ChangeState(EStatState::can);
		}
	}
}
