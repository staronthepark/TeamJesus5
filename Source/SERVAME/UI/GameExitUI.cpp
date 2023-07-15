// Fill out your copyright notice in the Description page of Project Settings.


#include "GameExitUI.h"

void UGameExitUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	YesButton->OnClicked.AddDynamic(this, &UGameExitUI::YesButtonClicked);
	NoButton->OnClicked.AddDynamic(this, &UGameExitUI::NoButtonClicked);
}

void UGameExitUI::YesButtonClicked()
{
	FGenericPlatformMisc::RequestExit(true);
}

void UGameExitUI::NoButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}
