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

	YesButton->OnHovered.AddDynamic(this, &UGameExitUI::YesButtonHovered);
	YesButton->OnUnhovered.AddDynamic(this, &UGameExitUI::YesButtonUnhovered);
	NoButton->OnHovered.AddDynamic(this, &UGameExitUI::NoButtonHovered);
	NoButton->OnUnhovered.AddDynamic(this, &UGameExitUI::NoButtonUnhovered);

	Buttons.Add(YesButton);
	Buttons.Add(NoButton);
}

void UGameExitUI::Open()
{
	this->SetVisibility(ESlateVisibility::Visible);
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ChangeLanguage(GameInstance->language);

}

void UGameExitUI::Close()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	this->GetParent()->SetKeyboardFocus();
}

void UGameExitUI::YesButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}

void UGameExitUI::NoButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	this->GetParent()->SetKeyboardFocus();
	Close();
}

void UGameExitUI::YesButtonHovered()
{
	YesButton->WidgetStyle.Normal.SetResourceObject(ImageTextures.Find(EExitSettings::yeshovered)->KorTexture);
	YesButton->WidgetStyle.Normal.ImageSize = FVector2D(69, 24);
}

void UGameExitUI::NoButtonHovered()
{
	NoButton->WidgetStyle.Normal.SetResourceObject(ImageTextures.Find(EExitSettings::nohovered)->KorTexture);
	NoButton->WidgetStyle.Hovered.ImageSize = FVector2D(69, 24);
}

void UGameExitUI::YesButtonUnhovered()
{
	YesButton->WidgetStyle.Normal.SetResourceObject(ImageTextures.Find(EExitSettings::yesnormal)->KorTexture);
	YesButton->WidgetStyle.Normal.ImageSize = FVector2D(69, 24);
}


void UGameExitUI::NoButtonUnhovered()
{
	NoButton->WidgetStyle.Normal.SetResourceObject(ImageTextures.Find(EExitSettings::nonormal)->KorTexture);
	NoButton->WidgetStyle.Normal.ImageSize = FVector2D(69, 24);
}

void UGameExitUI::ChangeLanguage(Language& language)
{
	if (language == Language::ENG)
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
	else if (language == Language::KOR)
	{
		PanelBackground->SetBrushFromTexture(ImageTextures.Find(EExitSettings::title)->KorTexture,true);
		YesButton->WidgetStyle.Normal.SetResourceObject(ImageTextures.Find(EExitSettings::yesnormal)->KorTexture);
		YesButton->WidgetStyle.Normal.ImageSize = FVector2D(69, 24);
		YesButton->WidgetStyle.Hovered.SetResourceObject(ImageTextures.Find(EExitSettings::yeshovered)->KorTexture);
		YesButton->WidgetStyle.Hovered.ImageSize = FVector2D(69, 24);
		YesButton->WidgetStyle.Pressed.SetResourceObject(ImageTextures.Find(EExitSettings::yespressed)->KorTexture);
		YesButton->WidgetStyle.Pressed.ImageSize = FVector2D(69, 24);
		NoButton->WidgetStyle.Normal.SetResourceObject(ImageTextures.Find(EExitSettings::nonormal)->KorTexture);
		NoButton->WidgetStyle.Normal.ImageSize = FVector2D(69, 24);
		NoButton->WidgetStyle.Hovered.SetResourceObject(ImageTextures.Find(EExitSettings::nohovered)->KorTexture);
		NoButton->WidgetStyle.Hovered.ImageSize = FVector2D(69, 24);
		NoButton->WidgetStyle.Pressed.SetResourceObject(ImageTextures.Find(EExitSettings::nopressed)->KorTexture);
		NoButton->WidgetStyle.Pressed.ImageSize = FVector2D(69, 24);
	}
}

FReply UGameExitUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();

	if (InKeyEvent.GetKey() == EKeys::Right || InKeyEvent.GetKey() == EKeys::Gamepad_DPad_Right)
	{
		Buttons[index]->OnUnhovered.Broadcast();
		index = FMath::Clamp(index + 1, 0, 1);
		Buttons[index]->OnHovered.Broadcast();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Left || InKeyEvent.GetKey() == EKeys::Gamepad_DPad_Left)
	{
		Buttons[index]->OnUnhovered.Broadcast();
		index = FMath::Clamp(index - 1, 0, 1);
		Buttons[index]->OnHovered.Broadcast();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Enter || InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
	{
		Buttons[index]->OnClicked.Broadcast();
		Reply = FReply::Handled();
	}

	return Reply;
}
