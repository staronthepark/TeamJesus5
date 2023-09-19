// Fill out your copyright notice in the Description page of Project Settings.

#include "StatUI.h"
#include "PlayerStatUI.h"

void UStatUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerStatComp = Cast<UPlayerStatComponent>(GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass(UPlayerStatComponent::StaticClass()));
	
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

	Button->WidgetStyle.Normal.SetResourceObject(ButtonStates.Find(EButtonState::normal)->Texture);
	Button->WidgetStyle.Hovered.SetResourceObject(ButtonStates.Find(EButtonState::hovered)->Texture);
	Button->WidgetStyle.Pressed.SetResourceObject(ButtonStates.Find(EButtonState::pressed)->Texture);
}

void UStatUI::ChangeState(EStatState changeState)
{
	state = changeState;
}

EStatState UStatUI::GetState()
{
	return state;
}

void UStatUI::OnButtonClicked()
{
	if (state == EStatState::closed || state == EStatState::activated)
		return;

	if(state == EStatState::can)
	{
		ParentUI->SelectedButton = this;
	}
}

void UStatUI::OnButtonUnclicked()
{
	ActiveBackground->SetVisibility(ESlateVisibility::Collapsed);
}

void UStatUI::Activate()
{
	state = EStatState::activated;
	ActiveBackground->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	TypeAnimation[Type](index);

	for (int i = 0; i < Lines.Num(); i++)
		Lines[i]->SetBrushFromTexture(LineTexture, true);

	if (NextStat == NULL)
		return;
	NextStat->index = index + 1;
	NextStat->ChangeState(EStatState::can);
}
