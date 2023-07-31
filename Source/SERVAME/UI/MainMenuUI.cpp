// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUI.h"


void UMainMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//UserSettingUI = Cast<UUserSettingUI>(CreateWidget(GetWorld(), UserSettingUIClass));
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	//LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, PlaybackSettings, LevelSequenceActor);
	//LevelSequencePlayer->OnFinished.AddDynamic(this, &UMainMenuUI::SequenceFinish);

	StartButton->OnHovered.AddDynamic(this, &UMainMenuUI::HoverStartButton);
	//SettingButton->OnHovered.AddDynamic(this, &UMainMenuUI::HoverSettingButton);
	QuitButton->OnHovered.AddDynamic(this, &UMainMenuUI::HoverQuitButton);

	StartButton->OnUnhovered.AddDynamic(this, &UMainMenuUI::UnhoverStartButton);
	//SettingButton->OnUnhovered.AddDynamic(this, &UMainMenuUI::UnhoverSettingButton);
	QuitButton->OnUnhovered.AddDynamic(this, &UMainMenuUI::UnhoverQuitButton);

	StartButton->OnClicked.AddDynamic(this, &UMainMenuUI::ClickStartButton);
	//SettingButton->OnClicked.AddDynamic(this, &UMainMenuUI::ClickSettingButton);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuUI::ClickQuitButton);
}

void UMainMenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(FadeOutAnimation);
}

void UMainMenuUI::HoverStartButton()
{
	StartBackgroundImage->SetRenderOpacity(1.0f);
}

//void UMainMenuUI::HoverSettingButton()
//{
//	SettingBackgroundImage->SetRenderOpacity(1.f);
//}


void UMainMenuUI::HoverQuitButton()
{
	QuitBackgroundImage->SetRenderOpacity(1.f);
}

void UMainMenuUI::UnhoverStartButton()
{
	StartBackgroundImage->SetRenderOpacity(.0f);
}

//void UMainMenuUI::UnhoverSettingButton()
//{
//	SettingBackgroundImage->SetRenderOpacity(0.f);
//}

void UMainMenuUI::UnhoverQuitButton()
{
	QuitBackgroundImage->SetRenderOpacity(0.f);
}


void UMainMenuUI::ClickStartButton()
{
	PlayAnimation(MainMenuCloseAnimation);
	RenderTargetImage->SetVisibility(ESlateVisibility::Collapsed);
	//if(LevelSequencePlayer)
	//	LevelSequencePlayer->Play();
}

//void UMainMenuUI::ClickSettingButton()
//{
//	if (UserSettingUI && !UserSettingUI->IsInViewport())
//		UserSettingUI->AddToViewport();
//}

void UMainMenuUI::ClickQuitButton()
{
	//if (UserSettingUI)
	//	UserSettingUI->ClickQuitSettingButton();
	UMG_GameExit->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuUI::SequenceFinish()
{
	if (this->IsInViewport())
		this->RemoveFromParent();
}





