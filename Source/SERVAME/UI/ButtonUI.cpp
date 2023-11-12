// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonUI.h"
#include <SERVAME/Manager/JesusGameInstance.h>

void UButtonUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	LeftButton->OnClicked.AddDynamic(this, &UButtonUI::LeftButtonClicked);
	RightButton->OnClicked.AddDynamic(this, &UButtonUI::RightButtonClicked);
	index = 0;

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
