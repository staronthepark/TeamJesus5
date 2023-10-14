// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialUI.h"
#include <Components/Image.h>
#include <SERVAME/Manager/JesusGameInstance.h>

void UTutorialUI::ChangeTexture(EGuides type, int isPad)
{
	Language currentLanguage = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->language;
	if (isPad == 0)
	{
		if (currentLanguage == Language::KOR)
			TutorialImage->SetBrushFromTexture(textures.Find(type)->KorPad, true);
		else
			TutorialImage->SetBrushFromTexture(textures.Find(type)->EngPad, true);
	}

	else if (isPad == 1)
	{
		if (currentLanguage == Language::KOR)
			TutorialImage->SetBrushFromTexture(textures.Find(type)->KorPc, true);
		else
			TutorialImage->SetBrushFromTexture(textures.Find(type)->EngPc, true);

	}

}
