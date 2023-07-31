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
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::hovered)->EngTextures);
	}
	else if (GameInstance->language == Language::KOR) 
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::hovered)->KorTextures);
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
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::unhovered)->EngTextures);
	}
	else if (GameInstance->language == Language::KOR)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::unhovered)->KorTextures);
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
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::hovered)->EngTextures);
	}
	else if (GameInstance->language == Language::KOR)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::hovered)->KorTextures);
	}
}

void USelectTextUI::OnUnhovered()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->language == Language::ENG)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::unhovered)->EngTextures);
	}
	else if (GameInstance->language == Language::KOR)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::unhovered)->KorTextures);
	}
}

void USelectTextUI::ChangeLanguage()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->language == Language::ENG)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::unhovered)->EngTextures);
	}
	else if (GameInstance->language == Language::KOR)
	{
		Text->SetBrushFromTexture(SelectTextTextures.Find(EHoverTextures::unhovered)->KorTextures);
	}
}


