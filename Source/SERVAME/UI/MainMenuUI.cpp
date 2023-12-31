// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUI.h"
#include <SERVAME/Player/JesusPlayerController.h>


void UMainMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//UserSettingUI = Cast<UUserSettingUI>(CreateWidget(GetWorld(), UserSettingUIClass));
	//FMovieSceneSequencePlaybackSettings PlaybackSettings;
	//LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, PlaybackSettings, LevelSequenceActor);
	//LevelSequencePlayer->OnFinished.AddDynamic(this, &UMainMenuUI::SequenceFinish);

	StartButton->OnHovered.AddDynamic(this, &UMainMenuUI::HoverStartButton);
	//SettingButton->OnHovered.AddDynamic(this, &UMainMenuUI::HoverSettingButton);
	ContinueButton->OnHovered.AddDynamic(this, &UMainMenuUI::HoverContinueButton);
	OptionButton->OnHovered.AddDynamic(this, &UMainMenuUI::HoverOptionButton);
	CreditButton->OnHovered.AddDynamic(this, &UMainMenuUI::HoverCreditButton);
	QuitButton->OnHovered.AddDynamic(this, &UMainMenuUI::HoverQuitButton);

	StartButton->OnUnhovered.AddDynamic(this, &UMainMenuUI::UnhoverStartButton);
	ContinueButton->OnUnhovered.AddDynamic(this, &UMainMenuUI::UnhoverContinueButton);
	OptionButton->OnUnhovered.AddDynamic(this, &UMainMenuUI::UnhoverOptionButton);
	CreditButton->OnUnhovered.AddDynamic(this, &UMainMenuUI::UnhoverCreditButton);
	//SettingButton->OnUnhovered.AddDynamic(this, &UMainMenuUI::UnhoverSettingButton);
	QuitButton->OnUnhovered.AddDynamic(this, &UMainMenuUI::UnhoverQuitButton);


	StartButton->OnClicked.AddDynamic(this, &UMainMenuUI::ClickStartButton);
	//SettingButton->OnClicked.AddDynamic(this, &UMainMenuUI::ClickSettingButton);
	ContinueButton->OnClicked.AddDynamic(this, &UMainMenuUI::ClickContinueButton);
	OptionButton->OnClicked.AddDynamic(this, &UMainMenuUI::ClickOptionButton);
	CreditButton->OnClicked.AddDynamic(this, &UMainMenuUI::ClickCreditButton);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuUI::ClickQuitButton);

	EndFadeInDelegate.BindDynamic(this, &UMainMenuUI::PlayFadeOutAnimation);
	BindToAnimationFinished(FadeInAnimation, EndFadeInDelegate);

	EndFadeOutDelegate.BindDynamic(this, &UMainMenuUI::OnEndFadeOut);
	BindToAnimationFinished(FadeOutAnimation, EndFadeOutDelegate);

	Buttons.Add(StartButton);
	Buttons.Add(ContinueButton);
	Buttons.Add(OptionButton);
	Buttons.Add(CreditButton);
	Buttons.Add(QuitButton);
	index = 0;
}

void UMainMenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(FadeOutAnimation);
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	Controller->SetInputMode(FInputModeUIOnly());
	Controller->bShowMouseCursor = true;
	this->SetKeyboardFocus();

}

void UMainMenuUI::HoverStartButton()
{
	StartBackgroundImage->SetRenderOpacity(1.0f);
}

void UMainMenuUI::HoverContinueButton()
{
	ContinueBackgroundImage->SetRenderOpacity(1.0f);
}

void UMainMenuUI::HoverOptionButton()
{
	OptionBackgroundImage->SetRenderOpacity(1.0f);
}

void UMainMenuUI::HoverCreditButton()
{
	CreditBackgroundImage->SetRenderOpacity(1.0f);
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

void UMainMenuUI::UnhoverContinueButton()
{
	ContinueBackgroundImage->SetRenderOpacity(0.0f);
}

void UMainMenuUI::UnhoverOptionButton()
{
	OptionBackgroundImage->SetRenderOpacity(0.0f);
}

//void UMainMenuUI::UnhoverSettingButton()
//{
//	SettingBackgroundImage->SetRenderOpacity(0.f);
//}

void UMainMenuUI::UnhoverCreditButton()
{
	CreditBackgroundImage->SetRenderOpacity(0.0f);
}

void UMainMenuUI::UnhoverQuitButton()
{
	QuitBackgroundImage->SetRenderOpacity(0.f);
}


void UMainMenuUI::ClickStartButton()
{
	PlayAnimation(FadeInAnimation);
	AJesusPlayerController* Controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->SetInputMode(FInputModeGameOnly());
	Controller->bShowMouseCursor = false;
	Controller->SetPause(false);
}

void UMainMenuUI::ClickContinueButton()
{
	PlayAnimation(MainMenuCloseAnimation);
	AJesusPlayerController* Controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->SetInputMode(FInputModeGameOnly());
	Controller->bShowMouseCursor = false;
	Controller->SetPause(false);
}

void UMainMenuUI::ClickOptionButton()
{
	AJesusPlayerController* Controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->character->UserSettingUI->AddToViewport();
	
}

void UMainMenuUI::ClickCreditButton()
{
	PlayAnimation(CreditAnimation);
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
	UMG_GameExit->Open();
	UMG_GameExit->SetKeyboardFocus();
}

void UMainMenuUI::PlayFadeOutAnimation()
{
	PlayAnimation(FadeOutForNewGame);
}

void UMainMenuUI::OnEndFadeOut()
{
}

FReply UMainMenuUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();

	if (InKeyEvent.GetKey() == EKeys::Down || InKeyEvent.GetKey() == EKeys::Gamepad_DPad_Down)
	{
		Buttons[index]->OnUnhovered.Broadcast();
		index = FMath::Clamp(index + 1, 0, Buttons.Num() - 1);
		Buttons[index]->OnHovered.Broadcast();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Up || InKeyEvent.GetKey() == EKeys::Gamepad_DPad_Up)
	{
		Buttons[index]->OnUnhovered.Broadcast();
		index = FMath::Clamp(index - 1, 0, Buttons.Num() - 1);
		Buttons[index]->OnHovered.Broadcast();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Enter || InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
	{
		Buttons[index]->OnClicked.Broadcast();
		Reply = FReply::Handled();
	}
	return Reply;
}

//void UMainMenuUI::SequenceFinish()
//{
//	if (this->IsInViewport())
//		this->RemoveFromParent();
//}





