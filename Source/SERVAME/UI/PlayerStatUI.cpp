// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatUI.h"
#include "Components/Image.h"
#include "StatUI.h"
#include "PlayerSoulStatUI.h"
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

	ButtonArray.Add(StrButtons);
	ButtonArray.Add(StaminaButtons);
	ButtonArray.Add(HpButtons);
	ButtonArray.Add(ShieldButtons);

	UpDownIndex = 0;
	LeftRightIndex = 0;

	for (int i = 0; i < StrButtons.Num(); i++)
	{
		//StrButtons[i]->Button->OnClicked.AddDynamic(this, &UPlayerStatUI::OnStrButtonClicked);
		StrButtons[i]->ParentUI = this;
	}

	for (int i = 0; i < StaminaButtons.Num(); i++)
	{
		//StaminaButtons[i]->Button->OnClicked.AddDynamic(this, &UPlayerStatUI::OnStaminaButtonClicked);
		StaminaButtons[i]->ParentUI = this;
	}

	for (int i = 0; i < HpButtons.Num(); i++)
	{
		//HpButtons[i]->Button->OnClicked.AddDynamic(this, &UPlayerStatUI::OnHpButtonClicked);
		HpButtons[i]->ParentUI = this;
	}

	for (int i = 0; i < ShieldButtons.Num(); i++)
	{
		//ShieldButtons[i]->Button->OnClicked.AddDynamic(this, &UPlayerStatUI::OnShieldButtonClicked);
		ShieldButtons[i]->ParentUI = this;
	}

	PurchaseButton->OnClicked.AddDynamic(this, &UPlayerStatUI::OnPurchaseButtonClicked);
	PurchaseButton->OnHovered.AddDynamic(this, &UPlayerStatUI::OnPurchaseButtonHovered);
	PurchaseButton->OnUnhovered.AddDynamic(this, &UPlayerStatUI::OnPurchaseButtonUnhovered);

}

void UPlayerStatUI::NativeConstruct()
{
	Super::NativeConstruct();
}

//void UPlayerStatUI::OnStrButtonClicked()
//{
//	ExplainTitle->SetBrushFromTexture(ExplainTextures.Find(EStatsType::str)->TitleTexture, true);
//	ExplainIcon->SetBrushFromTexture(ExplainTextures.Find(EStatsType::str)->IconTexture, true);
//	ExplainText->SetBrushFromTexture(ExplainTextures.Find(EStatsType::str)->ExplainText, true);
//}
//
//void UPlayerStatUI::OnStaminaButtonClicked()
//{
//	ExplainTitle->SetBrushFromTexture(ExplainTextures.Find(EStatsType::stamina)->TitleTexture, true);
//	ExplainIcon->SetBrushFromTexture(ExplainTextures.Find(EStatsType::stamina)->IconTexture, true);
//	ExplainText->SetBrushFromTexture(ExplainTextures.Find(EStatsType::stamina)->ExplainText, true);
//}
//
//void UPlayerStatUI::OnHpButtonClicked()
//{
//	ExplainTitle->SetBrushFromTexture(ExplainTextures.Find(EStatsType::hp)->TitleTexture, true);
//	ExplainIcon->SetBrushFromTexture(ExplainTextures.Find(EStatsType::hp)->IconTexture, true);
//	ExplainText->SetBrushFromTexture(ExplainTextures.Find(EStatsType::hp)->ExplainText, true);
//}
//
//void UPlayerStatUI::OnShieldButtonClicked()
//{
//	ExplainTitle->SetBrushFromTexture(ExplainTextures.Find(EStatsType::shield)->TitleTexture, true);
//	ExplainIcon->SetBrushFromTexture(ExplainTextures.Find(EStatsType::shield)->IconTexture, true);
//	ExplainText->SetBrushFromTexture(ExplainTextures.Find(EStatsType::shield)->ExplainText, true);
//}

void UPlayerStatUI::Init(int str, int stamina, int hp, int shield)
{
	for (int i = 0; i < str; i++)
		StrButtons[i]->Init();
	for (int i = 0; i < stamina; i++)
		StaminaButtons[i]->Init();
	for (int i = 0; i < hp; i++)
		HpButtons[i]->Init();
	for (int i = 0; i < shield; i++)
		ShieldButtons[i]->Init();
}

void UPlayerStatUI::OnPurchaseButtonHovered()
{
	PurchaseHoverButton->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerStatUI::OnPurchaseButtonUnhovered()
{
	PurchaseHoverButton->SetVisibility(ESlateVisibility::Collapsed);
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

void UPlayerStatUI::PlayActivatedAnimation()
{
	PlayAnimation(StatSucceededAnimation);
}

void UPlayerStatUI::SetCost(int value)
{
	CostText->SetText(FText::AsNumber(value));
}

void UPlayerStatUI::SetValue(int value, float x, float y)
{
	ValueText->SetText(FText::AsNumber(value));
	Cast<UCanvasPanelSlot>(ValueText->Slot)->SetPosition(FVector2D(x, y));
}

void UPlayerStatUI::Open()
{
	this->SetVisibility(ESlateVisibility::Visible);
	SucceededImage->SetVisibility(ESlateVisibility::Collapsed);
	//AJesusPlayerController* Controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	//Controller->SetInputMode(FInputModeUIOnly());
	//Controller->bShowMouseCursor = true;
}

void UPlayerStatUI::Close()
{
	GetParent()->GetChildAt(0)->SetKeyboardFocus();
	SucceededImage->SetVisibility(ESlateVisibility::Collapsed);
	this->SetVisibility(ESlateVisibility::Collapsed);
	//AJesusPlayerController* Controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	//Controller->SetInputMode(FInputModeGameOnly());
	//Controller->bShowMouseCursor = false;

	//AJesusPlayerController* Controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	//Controller->SetInputMode(FInputModeGameOnly());
	//Controller->bShowMouseCursor = false;
	//Controller->SetPause(false);
	//this->RemoveFromParent();

}

void UPlayerStatUI::SetExplainTitle(UTexture2D* texture)
{
	ExplainTitle->SetBrushFromTexture(texture, true);
}

void UPlayerStatUI::SetExplainIcon(UTexture2D* texture)
{
	ExplainIcon->SetBrushFromTexture(texture, true);
}

void UPlayerStatUI::SetExplainText(UTexture2D* texture)
{
	ExplainText->SetBrushFromTexture(texture, true);
}

FReply UPlayerStatUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();

	if (InKeyEvent.GetKey() == EKeys::Up)
	{
		ButtonArray[UpDownIndex][LeftRightIndex]->Button->OnUnhovered.Broadcast();
		UpDownIndex = FMath::Clamp(UpDownIndex - 1, 0, ButtonArray.Num() - 1);
		ButtonArray[UpDownIndex][LeftRightIndex]->Button->OnHovered.Broadcast();
		ButtonArray[UpDownIndex][LeftRightIndex]->Button->OnClicked.Broadcast();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Down)
	{
		ButtonArray[UpDownIndex][LeftRightIndex]->Button->OnUnhovered.Broadcast();
		UpDownIndex = FMath::Clamp(UpDownIndex + 1, 0, ButtonArray.Num() - 1);
		ButtonArray[UpDownIndex][LeftRightIndex]->Button->OnHovered.Broadcast();
		ButtonArray[UpDownIndex][LeftRightIndex]->Button->OnClicked.Broadcast();
		Reply = FReply::Handled();
	}
	
	if (InKeyEvent.GetKey() == EKeys::Right)
	{
		ButtonArray[UpDownIndex][LeftRightIndex]->Button->OnUnhovered.Broadcast();
		LeftRightIndex = FMath::Clamp(LeftRightIndex + 1, 0, ButtonArray[UpDownIndex].Num() - 1);
		ButtonArray[UpDownIndex][LeftRightIndex]->Button->OnHovered.Broadcast();
		ButtonArray[UpDownIndex][LeftRightIndex]->Button->OnClicked.Broadcast();
		Reply = FReply::Handled();
	}
	
	if (InKeyEvent.GetKey() == EKeys::Left)
	{
		ButtonArray[UpDownIndex][LeftRightIndex]->Button->OnUnhovered.Broadcast();
		LeftRightIndex = FMath::Clamp(LeftRightIndex - 1, 0, ButtonArray[UpDownIndex].Num() - 1);
		ButtonArray[UpDownIndex][LeftRightIndex]->Button->OnHovered.Broadcast();
		ButtonArray[UpDownIndex][LeftRightIndex]->Button->OnClicked.Broadcast();
		Reply = FReply::Handled();
	}
	
	if (InKeyEvent.GetKey() == EKeys::Enter)
	{
		PurchaseButton->OnClicked.Broadcast();
		Reply = FReply::Handled();
	}


	if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::Q || InKeyEvent.GetKey() == EKeys::Platform_Delete)
	{
		Close();
		Reply = FReply::Handled();
	}
	return Reply;
}

