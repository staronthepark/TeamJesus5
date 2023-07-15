// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectTextUI.h"

#define Unselected 0
#define Selected 1

void USelectTextUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Unselect();
	Button->OnHovered.AddDynamic(this, &USelectTextUI::OnHovered);
	Button->OnUnhovered.AddDynamic(this, &USelectTextUI::OnUnhovered);
}

void USelectTextUI::Select()
{
	Text->SetBrushFromTexture(TextIcons[Selected]);
	SelectedBackgroundImage->SetRenderOpacity(Selected);
	LeftButton->SetRenderOpacity(Selected);
	RightButton->SetRenderOpacity(Selected);
}

void USelectTextUI::Unselect()
{
	Text->SetBrushFromTexture(TextIcons[Unselected]);
	SelectedBackgroundImage->SetRenderOpacity(Unselected);
	LeftButton->SetRenderOpacity(Unselected);
	RightButton->SetRenderOpacity(Unselected);
}

void USelectTextUI::OnHovered()
{
	Text->SetBrushFromTexture(TextIcons[Selected]);
}

void USelectTextUI::OnUnhovered()
{
	Text->SetBrushFromTexture(TextIcons[Unselected]);
}

