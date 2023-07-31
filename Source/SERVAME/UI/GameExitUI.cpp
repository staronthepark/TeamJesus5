// Fill out your copyright notice in the Description page of Project Settings.


#include "GameExitUI.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

void UGameExitUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	YesButton->OnClicked.AddDynamic(this, &UGameExitUI::YesButtonClicked);
	NoButton->OnClicked.AddDynamic(this, &UGameExitUI::NoButtonClicked);
}

void UGameExitUI::YesButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}

void UGameExitUI::NoButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}
