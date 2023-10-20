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
}

void UPlayerSoulStatUI::ClickPray()
{
	Close();
}

void UPlayerSoulStatUI::OpenStat()
{
	//WBP_Player_Stat_UI->SetVisibility(ESlateVisibility::Visible);
	WBP_Player_Stat_UI->Open();
}

void UPlayerSoulStatUI::Open()
{
	this->SetVisibility(ESlateVisibility::Visible);
	AJesusPlayerController* Controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());
	this->SetKeyboardFocus();
}

void UPlayerSoulStatUI::Close()
{
	WBP_Player_Stat_UI->Close();
	this->SetVisibility(ESlateVisibility::Collapsed);
	AJesusPlayerController* Controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->SetInputMode(FInputModeGameOnly());
	Controller->bShowMouseCursor = false;
}

void UPlayerSoulStatUI::SetRemainSoul(int value)
{
	WBP_Player_Stat_UI->SetRemainSoul(value);
}

FReply UPlayerSoulStatUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();
	if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::Q)
	{
		Close();
		Reply = FReply::Handled();
	}
	return Reply;
}


