// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSoulStatUI.h"
#include <SERVAME/Player/JesusPlayerController.h>

void UPlayerSoulStatUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	WBP_PlayerRest_UI->PrayButton->OnClicked.AddDynamic(this, &UPlayerSoulStatUI::ClickPray);
	WBP_PlayerRest_UI->SoulButton->OnClicked.AddDynamic(this, &UPlayerSoulStatUI::OpenStat);
}

void UPlayerSoulStatUI::NativeConstruct()
{
	Super::NativeConstruct();
	this->SetVisibility(ESlateVisibility::Visible);
	WBP_PlayerRest_UI->SetVisibility(ESlateVisibility::Visible);
	AJesusPlayerController* Controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->SetPause(true);
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());
	WBP_PlayerRest_UI->SetKeyboardFocus();
}

void UPlayerSoulStatUI::ClickPray()
{
	WBP_PlayerRest_UI->SetVisibility(ESlateVisibility::Collapsed);
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	Controller->SetShowMouseCursor(false);
	Controller->SetInputMode(FInputModeGameOnly());
	Controller->SetPause(false);
	this->RemoveFromParent();
}

void UPlayerSoulStatUI::OpenStat()
{
	WBP_PlayerRest_UI->SetVisibility(ESlateVisibility::Collapsed);
	//WBP_Player_Stat_UI->SetVisibility(ESlateVisibility::Visible);
	WBP_Player_Stat_UI->Open();
}

void UPlayerSoulStatUI::Close()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	AJesusPlayerController* Controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->SetInputMode(FInputModeGameOnly());
	Controller->bShowMouseCursor = false;
	Controller->SetPause(false);
	WBP_Player_Stat_UI->Close();
	this->RemoveFromParent();
}

void UPlayerSoulStatUI::SetRemainSoul(int value)
{
	WBP_Player_Stat_UI->SetRemainSoul(value);
}

FReply UPlayerSoulStatUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		Close();
		Reply = FReply::Handled();
	}
	return Reply;
}


