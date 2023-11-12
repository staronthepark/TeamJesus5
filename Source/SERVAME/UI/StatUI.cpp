// Fill out your copyright notice in the Description page of Project Settings.

#include "StatUI.h"
#include "PlayerStatUI.h"

void UStatUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerStatComp = Cast<UPlayerStatComponent>(GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass(UPlayerStatComponent::StaticClass()));
	
	Button->OnClicked.AddDynamic(this, &UStatUI::OnButtonClicked);
	Button->OnHovered.AddDynamic(this, &UStatUI::OnButtonHovered);
	Button->OnUnhovered.AddDynamic(this, &UStatUI::OnButtonUnhovered);

	TypeAnimation.Add(EStateType::str, [&](int32 index) {
		return PlayerStatComp->StrengthStatList[index].Func();
		});
	TypeAnimation.Add(EStateType::stamina, [&](int32 index) {
		return PlayerStatComp->StaminaStatList[index].Func();
		});
	TypeAnimation.Add(EStateType::hp, [&](int32 index) {
		return PlayerStatComp->HpStatList[index].Func();
		});
	TypeAnimation.Add(EStateType::shield, [&](int32 index) {
		return PlayerStatComp->ShieldStatList[index].Func();
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

void UStatUI::Init()
{
	state = EStatState::activated;
	ActiveBackground->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ChangeButtonToActivate();

	for (int i = 0; i < Lines.Num(); i++)
		Lines[i]->SetBrushFromTexture(LineTexture, true);

	if (NextStat == NULL)
		return;
	NextStat->index = index + 1;
	NextStat->ChangeState(EStatState::can);
}

void UStatUI::OnButtonHovered()
{
	Button->WidgetStyle.Normal.SetResourceObject(ButtonStates.Find(EButtonState::hovered)->Texture);

}

void UStatUI::OnButtonUnhovered()
{
	Button->WidgetStyle.Normal.SetResourceObject(ButtonStates.Find(EButtonState::normal)->Texture);
}

void UStatUI::OnButtonClicked()
{
	ParentUI->SetCost(Cost);
	ParentUI->SetValue(Value, x, y);

	ParentUI->SetExplainTitle(ExplainTitle);
	ParentUI->SetExplainIcon(ExplainIcon);
	ParentUI->SetExplainText(ExplainText);

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
	if (TypeAnimation[Type](index)) {
		Init();
		ParentUI->PlayActivatedAnimation();
	}
}

void UStatUI::ChangeButtonToActivate()
{
	Button->WidgetStyle.Normal.SetResourceObject(ButtonStates.Find(EButtonState::pressed)->Texture);
	Button->WidgetStyle.Hovered.SetResourceObject(ButtonStates.Find(EButtonState::hovered)->Texture);
	Button->WidgetStyle.Pressed.SetResourceObject(ButtonStates.Find(EButtonState::pressed)->Texture);
}
