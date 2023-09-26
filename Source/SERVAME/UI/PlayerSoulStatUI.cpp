// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSoulStatUI.h"

void UPlayerSoulStatUI::NativeOnInitialized()
{
	WBP_PlayerRest_UI->PrayButton->OnClicked.AddDynamic(this, &UPlayerSoulStatUI::ClickPray);
	WBP_PlayerRest_UI->SoulButton->OnClicked.AddDynamic(this, &UPlayerSoulStatUI::OpenStat);
}

void UPlayerSoulStatUI::ClickPray()
{
	WBP_PlayerRest_UI->SetVisibility(ESlateVisibility::Collapsed);
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	Controller->SetShowMouseCursor(false);
	Controller->SetInputMode(FInputModeGameOnly());
}

void UPlayerSoulStatUI::OpenStat()
{
	WBP_PlayerRest_UI->SetVisibility(ESlateVisibility::Collapsed);
	WBP_Player_Stat_UI->SetVisibility(ESlateVisibility::Visible);
}
