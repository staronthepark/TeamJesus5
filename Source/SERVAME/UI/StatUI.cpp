// Fill out your copyright notice in the Description page of Project Settings.


#include "StatUI.h"

void UStatUI::NativeOnInitialized()
{
	Button->OnClicked.AddDynamic(this, &UStatUI::OnButtonClicked);
	TypeAnimation.Add(EStateType::str, [&]() {

		});
	TypeAnimation.Add(EStateType::stamina, [&]() {

		});
	TypeAnimation.Add(EStateType::hp, [&]() {

		});
	TypeAnimation.Add(EStateType::shield, [&]() {

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
		TypeAnimation[Type]();

		for (int i = 0; i < NextStat.Num(); i++)
		{
			NextStat[i]->ChangeState(EStatState::can);
		}
	}
}
