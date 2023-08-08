// Fill out your copyright notice in the Description page of Project Settings.


#include "GameExitUI.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <SERVAME/Manager/JesusGameInstance.h>

void UGameExitUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	YesButton->OnClicked.AddDynamic(this, &UGameExitUI::YesButtonClicked);
	NoButton->OnClicked.AddDynamic(this, &UGameExitUI::NoButtonClicked);
	ChangeLanguage();
}

void UGameExitUI::YesButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}

void UGameExitUI::NoButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UGameExitUI::ChangeLanguage()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->language == Language::ENG)
	{
		PanelBackground->SetBrushFromTexture(ImageTextures.Find(EExitSettings::title)->EngTexture,true);
		YesButton->WidgetStyle.Normal.SetResourceObject(ImageTextures.Find(EExitSettings::yesnormal)->EngTexture);
		YesButton->WidgetStyle.Normal.ImageSize = FVector2D(45, 22);
		YesButton->WidgetStyle.Hovered.SetResourceObject(ImageTextures.Find(EExitSettings::yeshovered)->EngTexture);
		YesButton->WidgetStyle.Hovered.ImageSize = FVector2D(45, 22);
		YesButton->WidgetStyle.Pressed.SetResourceObject(ImageTextures.Find(EExitSettings::yespressed)->EngTexture);
		YesButton->WidgetStyle.Pressed.ImageSize = FVector2D(45, 22);
		NoButton->WidgetStyle.Normal.SetResourceObject(ImageTextures.Find(EExitSettings::nonormal)->EngTexture);
		NoButton->WidgetStyle.Normal.ImageSize = FVector2D(38, 23);
		NoButton->WidgetStyle.Hovered.SetResourceObject(ImageTextures.Find(EExitSettings::nohovered)->EngTexture);
		NoButton->WidgetStyle.Hovered.ImageSize = FVector2D(38, 23);
		NoButton->WidgetStyle.Pressed.SetResourceObject(ImageTextures.Find(EExitSettings::nopressed)->EngTexture);
		NoButton->WidgetStyle.Pressed.ImageSize = FVector2D(38, 23);
	}
	else if (GameInstance->language == Language::KOR)
	{
		PanelBackground->SetBrushFromTexture(ImageTextures.Find(EExitSettings::title)->KorTexture,true);
		YesButton->WidgetStyle.Normal.SetResourceObject(ImageTextures.Find(EExitSettings::yesnormal)->KorTexture);
		YesButton->WidgetStyle.Normal.ImageSize = FVector2D(93, 24);
		YesButton->WidgetStyle.Hovered.SetResourceObject(ImageTextures.Find(EExitSettings::yeshovered)->KorTexture);
		YesButton->WidgetStyle.Hovered.ImageSize = FVector2D(93, 24);
		YesButton->WidgetStyle.Pressed.SetResourceObject(ImageTextures.Find(EExitSettings::yespressed)->KorTexture);
		YesButton->WidgetStyle.Pressed.ImageSize = FVector2D(93, 24);
		NoButton->WidgetStyle.Normal.SetResourceObject(ImageTextures.Find(EExitSettings::nonormal)->KorTexture);
		NoButton->WidgetStyle.Normal.ImageSize = FVector2D(93, 24);
		NoButton->WidgetStyle.Hovered.SetResourceObject(ImageTextures.Find(EExitSettings::nohovered)->KorTexture);
		NoButton->WidgetStyle.Hovered.ImageSize = FVector2D(93, 24);
		NoButton->WidgetStyle.Pressed.SetResourceObject(ImageTextures.Find(EExitSettings::nopressed)->KorTexture);
		NoButton->WidgetStyle.Pressed.ImageSize = FVector2D(93, 24);
	}
}
