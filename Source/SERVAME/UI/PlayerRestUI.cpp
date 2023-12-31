// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRestUI.h"

void UPlayerRestUI::NativeOnInitialized()
{
	PrayButton->OnUnhovered.AddDynamic(this, &UPlayerRestUI::OnPrayButtonUnhovered);
	PrayButton->OnHovered.AddDynamic(this, &UPlayerRestUI::OnPrayButtonHovered);
	SoulButton->OnUnhovered.AddDynamic(this, &UPlayerRestUI::OnSoulButtonUnhovered);
	SoulButton->OnHovered.AddDynamic(this, &UPlayerRestUI::OnSoulButtonHovered);

	PrayButton->OnClicked.AddDynamic(this, &UPlayerRestUI::OnPrayButtonClicked);
	SoulButton->OnClicked.AddDynamic(this, &UPlayerRestUI::OnSoulButtonClicked);

	Buttons.Add(SoulButton);
	Buttons.Add(PrayButton);

	OnPrayButtonUnhovered();
	OnSoulButtonUnhovered();
}

void UPlayerRestUI::NativeConstruct()
{
	Super::NativeConstruct();
	Open();
}

void UPlayerRestUI::OnPrayButtonUnhovered()
{
	if (currentLanguage == Language::ENG)
	{
		PrayImage->SetBrushFromTexture(ButtonTextures.Find(ERestButtons::PrayNormal)->EngTextures, true);
	}
	else if (currentLanguage == Language::KOR)
	{
		PrayImage->SetBrushFromTexture(ButtonTextures.Find(ERestButtons::PrayNormal)->KorTextures, true);
	}
	PrayBackgroundHovered->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerRestUI::OnPrayButtonHovered()
{
	if (currentLanguage == Language::ENG)
	{
		PrayImage->SetBrushFromTexture(ButtonTextures.Find(ERestButtons::PrayHovered)->EngTextures, true);
	}
	else if (currentLanguage == Language::KOR)
	{
		PrayImage->SetBrushFromTexture(ButtonTextures.Find(ERestButtons::PrayHovered)->KorTextures, true);
	}
	PrayBackgroundHovered->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPlayerRestUI::OnSoulButtonUnhovered()
{
	if (currentLanguage == Language::ENG)
	{
		SoulImage->SetBrushFromTexture(ButtonTextures.Find(ERestButtons::SoulNormal)->EngTextures, true);
	}
	else if (currentLanguage == Language::KOR)
	{
		SoulImage->SetBrushFromTexture(ButtonTextures.Find(ERestButtons::SoulNormal)->KorTextures, true);
	}
	SoulBackgroundHovered->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerRestUI::OnSoulButtonHovered()
{
	if (currentLanguage == Language::ENG)
	{
		SoulImage->SetBrushFromTexture(ButtonTextures.Find(ERestButtons::SoulHovered)->EngTextures, true);
	}
	else if (currentLanguage == Language::KOR)
	{
		SoulImage->SetBrushFromTexture(ButtonTextures.Find(ERestButtons::SoulHovered)->KorTextures, true);
	}
	SoulBackgroundHovered->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPlayerRestUI::OnPrayButtonClicked()
{
}

void UPlayerRestUI::OnSoulButtonClicked()
{
	//this->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerRestUI::Open()
{
	currentLanguage = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->language;

	if (currentLanguage == Language::ENG)
	{
		PrayImage->SetBrushFromTexture(ButtonTextures.Find(ERestButtons::PrayNormal)->EngTextures, true);
		SoulImage->SetBrushFromTexture(ButtonTextures.Find(ERestButtons::SoulNormal)->EngTextures, true);
	}
	else {
		PrayImage->SetBrushFromTexture(ButtonTextures.Find(ERestButtons::PrayNormal)->KorTextures, true);
		SoulImage->SetBrushFromTexture(ButtonTextures.Find(ERestButtons::SoulNormal)->KorTextures, true);

	}
	//if (currentLanguage == Language::ENG)
	//{
	//	PrayButton->WidgetStyle.Normal.SetResourceObject(ButtonTextures.Find(ERestButtons::PrayNormal)->EngTextures);
	//	PrayButton->WidgetStyle.Hovered.SetResourceObject(ButtonTextures.Find(ERestButtons::PrayHovered)->EngTextures);
	//	PrayButton->WidgetStyle.Pressed.SetResourceObject(ButtonTextures.Find(ERestButtons::PrayHovered)->EngTextures);

	//	SoulButton->WidgetStyle.Normal.SetResourceObject(ButtonTextures.Find(ERestButtons::SoulNormal)->EngTextures);
	//	SoulButton->WidgetStyle.Hovered.SetResourceObject(ButtonTextures.Find(ERestButtons::SoulHovered)->EngTextures);
	//	SoulButton->WidgetStyle.Pressed.SetResourceObject(ButtonTextures.Find(ERestButtons::SoulHovered)->EngTextures);
	//}
	//else if (currentLanguage == Language::KOR)
	//{
	//	PrayButton->WidgetStyle.Normal.SetResourceObject(ButtonTextures.Find(ERestButtons::PrayNormal)->KorTextures);
	//	PrayButton->WidgetStyle.Hovered.SetResourceObject(ButtonTextures.Find(ERestButtons::PrayHovered)->KorTextures);
	//	PrayButton->WidgetStyle.Pressed.SetResourceObject(ButtonTextures.Find(ERestButtons::PrayHovered)->KorTextures);

	//	SoulButton->WidgetStyle.Normal.SetResourceObject(ButtonTextures.Find(ERestButtons::SoulNormal)->KorTextures);
	//	SoulButton->WidgetStyle.Hovered.SetResourceObject(ButtonTextures.Find(ERestButtons::SoulHovered)->KorTextures);
	//	SoulButton->WidgetStyle.Pressed.SetResourceObject(ButtonTextures.Find(ERestButtons::SoulHovered)->KorTextures);
	//}

	this->SetVisibility(ESlateVisibility::Visible);
}

FReply UPlayerRestUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();
	if (InKeyEvent.GetKey() == EKeys::Down || InKeyEvent.GetKey() == EKeys::Gamepad_DPad_Down)
	{
		Buttons[index]->OnUnhovered.Broadcast();
		index = FMath::Clamp(index + 1, 0, Buttons.Num() - 1);
		UE_LOG(LogTemp, Warning, TEXT("Index = %d"), index);
		Buttons[index]->OnHovered.Broadcast();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Up || InKeyEvent.GetKey() == EKeys::Gamepad_DPad_Up)
	{
		Buttons[index]->OnUnhovered.Broadcast();
		index = FMath::Clamp(index - 1, 0, Buttons.Num() - 1);
		UE_LOG(LogTemp, Warning, TEXT("Index = %d"), index);
		Buttons[index]->OnHovered.Broadcast();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Enter || InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
	{
		Buttons[index]->OnClicked.Broadcast();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Right)
	{
		Reply = FReply::Handled();
	}

	return Reply;
}
