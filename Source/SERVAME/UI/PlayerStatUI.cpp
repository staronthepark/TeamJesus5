// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatUI.h"

void UPlayerStatUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StrButtons.Add(StrButton0);
	StrButtons.Add(StrButton1);
	StrButtons.Add(StrButton2);
	StrButtons.Add(StrButton3);

	StaminaButtons.Add(StaminaButton0);
	StaminaButtons.Add(StaminaButton1);
	StaminaButtons.Add(StaminaButton2);
	StaminaButtons.Add(StaminaButton3);

	HpButtons.Add(HpButton0);
	HpButtons.Add(HpButton1);
	HpButtons.Add(HpButton2);
	HpButtons.Add(HpButton3);

	ShieldButtons.Add(ShieldButton0);
	ShieldButtons.Add(ShieldButton1);
	ShieldButtons.Add(ShieldButton2);
	ShieldButtons.Add(ShieldButton3);

	for (int i = 0; i < StrButtons.Num(); i++)
	{
		StrButtons[i]->Button->OnClicked.AddDynamic(this, &UPlayerStatUI::OnStrButtonClicked);
		StrButtons[i]->ParentUI = this;
	}

	for (int i = 0; i < StaminaButtons.Num(); i++)
	{
		StaminaButtons[i]->Button->OnClicked.AddDynamic(this, &UPlayerStatUI::OnStaminaButtonClicked);
		StaminaButtons[i]->ParentUI = this;
	}

	for (int i = 0; i < HpButtons.Num(); i++)
	{
		HpButtons[i]->Button->OnClicked.AddDynamic(this, &UPlayerStatUI::OnHpButtonClicked);
		HpButtons[i]->ParentUI = this;
	}

	for (int i = 0; i < ShieldButtons.Num(); i++)
	{
		ShieldButtons[i]->Button->OnClicked.AddDynamic(this, &UPlayerStatUI::OnShieldButtonClicked);
		ShieldButtons[i]->ParentUI = this;
	}

	PurchaseButton->OnClicked.AddDynamic(this, &UPlayerStatUI::OnPurchaseButtonClicked);
}

void UPlayerStatUI::OnStrButtonClicked()
{
	ExplainTitle->SetBrushFromTexture(ExplainTextures.Find(EStateType::str)->TitleTexture, true);
	ExplainIcon->SetBrushFromTexture(ExplainTextures.Find(EStateType::str)->IconTexture, true);
	ExplainText->SetBrushFromTexture(ExplainTextures.Find(EStateType::str)->ExplainText, true);
}

void UPlayerStatUI::OnStaminaButtonClicked()
{
	ExplainTitle->SetBrushFromTexture(ExplainTextures.Find(EStateType::stamina)->TitleTexture, true);
	ExplainIcon->SetBrushFromTexture(ExplainTextures.Find(EStateType::stamina)->IconTexture, true);
	ExplainText->SetBrushFromTexture(ExplainTextures.Find(EStateType::stamina)->ExplainText, true);
}

void UPlayerStatUI::OnHpButtonClicked()
{
	ExplainTitle->SetBrushFromTexture(ExplainTextures.Find(EStateType::hp)->TitleTexture, true);
	ExplainIcon->SetBrushFromTexture(ExplainTextures.Find(EStateType::hp)->IconTexture, true);
	ExplainText->SetBrushFromTexture(ExplainTextures.Find(EStateType::hp)->ExplainText, true);
}

void UPlayerStatUI::OnShieldButtonClicked()
{
	ExplainTitle->SetBrushFromTexture(ExplainTextures.Find(EStateType::shield)->TitleTexture, true);
	ExplainIcon->SetBrushFromTexture(ExplainTextures.Find(EStateType::shield)->IconTexture, true);
	ExplainText->SetBrushFromTexture(ExplainTextures.Find(EStateType::shield)->ExplainText, true);
}

void UPlayerStatUI::OnPurchaseButtonClicked()
{
	if (SelectedButton == nullptr || SelectedButton->GetState() != EStatState::can)
		return;

	SelectedButton->Activate();
}

void UPlayerStatUI::SetRemainSoul(int value)
{
	RemainSoulText->SetText(FText::AsNumber(value));
}
