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

	index = 0;
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
		if (NextStat == NULL)
			return;
		NextStat->index = index + 1;
		TypeAnimation[Type](index);


		NextStat->ChangeState(EStatState::can);

	}
}
