// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonUI.h"

void UButtonUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	LeftButton->OnClicked.AddDynamic(this, &UButtonUI::LeftButtonClicked);
	RightButton->OnClicked.AddDynamic(this, &UButtonUI::RightButtonClicked);
	index = 0;
	OnOffImage->SetBrushFromTexture(OnOffImages[index]);
}

void UButtonUI::LeftButtonClicked()
{
	index = FMath::Abs(--index % OnOffImages.Num());
	OnOffImage->SetBrushFromTexture(OnOffImages[index]);
}

void UButtonUI::RightButtonClicked()
{
	index = FMath::Abs(++index % OnOffImages.Num());
	OnOffImage->SetBrushFromTexture(OnOffImages[index]);
}

int UButtonUI::GetValue()
{
	return index;
}

void UButtonUI::SetValue(int value)
{
	index = FMath::Clamp(value, 0, OnOffImages.Num() - 1);
	OnOffImage->SetBrushFromTexture(OnOffImages[index]);
}
