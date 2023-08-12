// Fill out your copyright notice in the Description page of Project Settings.


#include "StatMarkUI.h"

void UStatMarkUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Button->OnClicked.AddDynamic(this, &UStatMarkUI::OnClicked);
}

void UStatMarkUI::NativeConstruct()
{
	Super::NativeConstruct();

}

void UStatMarkUI::SetStat(EState value)
{
	State = value;
}



void UStatMarkUI::OnClicked()
{
	if (State == EState::Deactive || State == EState::Active)
		return;

	else if (State == EState::CanActive)
	{
		State = EState::Active;
		for (int i = 0; i < NextMarks.Num(); i++)
		{
			NextMarks[i]->SetStat(EState::CanActive);
		}
	}
}
