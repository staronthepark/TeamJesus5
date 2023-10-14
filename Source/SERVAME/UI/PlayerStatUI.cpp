// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatUI.h"
#include "Components/Image.h"
#include "StatUI.h"
#include <SERVAME/Player/JesusPlayerController.h>


void UPlayerStatUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StrButtons.Add(StrButton0);
	StrButtons.Add(StrButton1);
	StrButtons.Add(StrButton2);
	StrButtons.Add(StrButton3);
	StrButtons.Add(StrButton4);

	StaminaButtons.Add(StaminaButton0);
	StaminaButtons.Add(StaminaButton1);
	StaminaButtons.Add(StaminaButton2);
	StaminaButtons.Add(StaminaButton3);
	StaminaButtons.Add(StaminaButton4);

	HpButtons.Add(HpButton0);
	HpButtons.Add(HpButton1);
	HpButtons.Add(HpButton2);
	HpButtons.Add(HpButton3);
	HpButtons.Add(HpButton4);

	ShieldButtons.Add(ShieldButton0);
	ShieldButtons.Add(ShieldButton1);
	ShieldButtons.Add(ShieldButton2);
	ShieldButtons.Add(ShieldButton3);
	ShieldButtons.Add(ShieldButton4);

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

void UPlayerStatUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerStatUI::OnStrButtonClicked()
{
	ExplainTitle->SetBrushFromTexture(ExplainTextures.Find(EStatsType::str)->TitleTexture, true);
	ExplainIcon->SetBrushFromTexture(ExplainTextures.Find(EStatsType::str)->IconTexture, true);
	ExplainText->SetBrushFromTexture(ExplainTextures.Find(EStatsType::str)->ExplainText, true);
}

void UPlayerStatUI::OnStaminaButtonClicked()
{
	ExplainTitle->SetBrushFromTexture(ExplainTextures.Find(EStatsType::stamina)->TitleTexture, true);
	ExplainIcon->SetBrushFromTexture(ExplainTextures.Find(EStatsType::stamina)->IconTexture, true);
	ExplainText->SetBrushFromTexture(ExplainTextures.Find(EStatsType::stamina)->ExplainText, true);
}

void UPlayerStatUI::OnHpButtonClicked()
{
	ExplainTitle->SetBrushFromTexture(ExplainTextures.Find(EStatsType::hp)->TitleTexture, true);
	ExplainIcon->SetBrushFromTexture(ExplainTextures.Find(EStatsType::hp)->IconTexture, true);
	ExplainText->SetBrushFromTexture(ExplainTextures.Find(EStatsType::hp)->ExplainText, true);
}

void UPlayerStatUI::OnShieldButtonClicked()
{
	ExplainTitle->SetBrushFromTexture(ExplainTextures.Find(EStatsType::shield)->TitleTexture, true);
	ExplainIcon->SetBrushFromTexture(ExplainTextures.Find(EStatsType::shield)->IconTexture, true);
	ExplainText->SetBrushFromTexture(ExplainTextures.Find(EStatsType::shield)->ExplainText, true);
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

void UPlayerStatUI::SetCost(int value)
{
	CostText->SetText(FText::AsNumber(value));
}

void UPlayerStatUI::Open()
{
	this->SetVisibility(ESlateVisibility::Visible);
	AJesusPlayerController* Controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->SetInputMode(FInputModeUIOnly());
	Controller->bShowMouseCursor = true;
	StrButton0->SetKeyboardFocus();
}

void UPlayerStatUI::Close()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	//AJesusPlayerController* Controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	//Controller->SetInputMode(FInputModeGameOnly());
	//Controller->bShowMouseCursor = false;
	//Controller->SetPause(false);
	//this->RemoveFromParent();

}

//FReply UPlayerStatUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
//{
//	FReply Reply = FReply::Unhandled();
//	if (InKeyEvent.GetKey() == EKeys::Escape)
//	{
//		Close();
//		Reply = FReply::Handled();
//	}
//	return Reply;
//}

