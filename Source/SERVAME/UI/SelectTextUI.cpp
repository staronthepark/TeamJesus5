// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectTextUI.h"
#include <SERVAME/Manager/JesusGameInstance.h>

#define Unselected 0
#define Selected 1

void USelectTextUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Button->OnHovered.AddDynamic(this, &USelectTextUI::OnHovered);
	Button->OnUnhovered.AddDynamic(this, &USelectTextUI::OnUnhovered);
}

void USelectTextUI::NativeConstruct()
{
	Super::NativeConstruct();
	Unselect();
}

void USelectTextUI::Select()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->language == Language::ENG) 
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::hovered)->EngTextures,true);
	}
	else if (GameInstance->language == Language::KOR) 
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::hovered)->KorTextures, true);
	}
	SelectedBackgroundImage->SetRenderOpacity(Selected);
	LeftButton->SetRenderOpacity(Selected);
	RightButton->SetRenderOpacity(Selected);
}

void USelectTextUI::Unselect()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->language == Language::ENG)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::unhovered)->EngTextures, true);
	}
	else if (GameInstance->language == Language::KOR)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::unhovered)->KorTextures, true);
	}
	SelectedBackgroundImage->SetRenderOpacity(Unselected);
	LeftButton->SetRenderOpacity(Unselected);
	RightButton->SetRenderOpacity(Unselected);
}

void USelectTextUI::OnHovered()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->language == Language::ENG)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::hovered)->EngTextures, true);
	}
	else if (GameInstance->language == Language::KOR)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::hovered)->KorTextures, true);
	}
}

void USelectTextUI::OnUnhovered()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->language == Language::ENG)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::unhovered)->EngTextures, true);
	}
	else if (GameInstance->language == Language::KOR)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::unhovered)->KorTextures, true);
	}
}

void USelectTextUI::ChangeLanguage(Language& language)
{
	if (language == Language::ENG)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::unhovered)->EngTextures, true);
	}
	else if (language == Language::KOR)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::unhovered)->KorTextures, true);
	}
}


