// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonUI.h"
#include <SERVAME/Manager/JesusGameInstance.h>

void UButtonUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	LeftButton->OnClicked.AddDynamic(this, &UButtonUI::LeftButtonClicked);
	RightButton->OnClicked.AddDynamic(this, &UButtonUI::RightButtonClicked);
	index = 0;

	LeftButton->OnHovered.AddDynamic(this, &UButtonUI::LeftButtonHovered);
	RightButton->OnHovered.AddDynamic(this, &UButtonUI::RightButtonHovered);

	LeftButton->OnUnhovered.AddDynamic(this, &UButtonUI::LeftButtonUnhovered);
	RightButton->OnUnhovered.AddDynamic(this, &UButtonUI::RightButtonUnhovered);

	Buttons.Add(LeftButton);
	Buttons.Add(RightButton);
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ChangeLanguage(GameInstance->language);
}

void UButtonUI::LeftButtonClicked()
{
	if (--index == -1)
		index = OnOffImages.Num() - 1;
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ChangeLanguage(GameInstance->language);
}

void UButtonUI::RightButtonClicked()
{
	index = ++index % OnOffImages.Num();
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ChangeLanguage(GameInstance->language);
}

int UButtonUI::GetValue()
{
	return index;
}

void UButtonUI::SetValue(int value)
{
	index = FMath::Clamp(value, 0, OnOffImages.Num() - 1);
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ChangeLanguage(GameInstance->language);
}

void UButtonUI::LeftButtonHovered()
{
	LeftButton->WidgetStyle.Normal.SetResourceObject(LeftButtonHoveredImages[0]);
}

void UButtonUI::RightButtonHovered()
{
	RightButton->WidgetStyle.Normal.SetResourceObject(RightButtonHoveredImages[0]);
}

void UButtonUI::LeftButtonUnhovered()
{
	LeftButton->WidgetStyle.Normal.SetResourceObject(LeftButtonHoveredImages[1]);
}

void UButtonUI::RightButtonUnhovered()
{
	RightButton->WidgetStyle.Normal.SetResourceObject(RightButtonHoveredImages[1]);
}

void UButtonUI::ChangeLanguage(Language& language)
{
	if (language == Language::ENG)
	{
		OnOffImage->SetBrushFromTexture(OnOffImages.Find(index)->EngTexture, true);
	}
	else if (language == Language::KOR)
	{
		OnOffImage->SetBrushFromTexture(OnOffImages.Find(index)->KorTexture, true);
	}
}
