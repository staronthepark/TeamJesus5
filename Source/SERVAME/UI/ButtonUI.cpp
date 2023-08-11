// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonUI.h"
#include <SERVAME/Manager/JesusGameInstance.h>

void UButtonUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	LeftButton->OnClicked.AddDynamic(this, &UButtonUI::LeftButtonClicked);
	RightButton->OnClicked.AddDynamic(this, &UButtonUI::RightButtonClicked);
	index = 0;
	ChangeLanguage();
}

void UButtonUI::LeftButtonClicked()
{
	index = FMath::Abs(--index % OnOffImages.Num());
	ChangeLanguage();
}

void UButtonUI::RightButtonClicked()
{
	index = FMath::Abs(++index % OnOffImages.Num());
	ChangeLanguage();
}

int UButtonUI::GetValue()
{
	return index;
}

void UButtonUI::SetValue(int value)
{
	index = FMath::Clamp(value, 0, OnOffImages.Num() - 1);
	ChangeLanguage();
}

void UButtonUI::ChangeLanguage()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->language == Language::ENG)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), index);
		OnOffImage->SetBrushFromTexture(OnOffImages.Find(index)->EngTexture, true);
	}
	else if (GameInstance->language == Language::KOR)
	{

		UE_LOG(LogTemp, Warning, TEXT("%d"), index);
		OnOffImage->SetBrushFromTexture(OnOffImages.Find(index)->KorTexture, true);
	}
}
