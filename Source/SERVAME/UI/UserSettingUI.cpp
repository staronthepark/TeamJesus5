// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettingUI.h"
#include "..\Manager\SoundManager.h"
#include <SERVAME/Manager/JesusGameInstance.h>
#include <SERVAME/Player/JesusPlayerController.h>

#define Game 0
#define Audio 1
#define Graphics 2
#define Key 3
#define Quit 4

void UUserSettingUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//GameExitUI = Cast<UGameExitUI>(CreateWidget(GetWorld(), GameExitUIClass));
	SelectSettingArray.Add(WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Camera); //0
	SelectSettingArray.Add(WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Audio); //1
	//SelectSettingArray.Add(WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Graphics); 
	SelectSettingArray.Add(WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Keyguide); //2
	SelectSettingArray.Add(WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Quit); //3

	SubUserSettingArray.Add(WBP_UserSetting_GameUI); // 0
	SubUserSettingArray.Add(WBP_UserSetting_AudioUI); // 1
	SubUserSettingArray.Add(WBP_UserSetting_GraphicsUI); //2

	SelectSettingArray[0]->Button->OnClicked.AddDynamic(this, &UUserSettingUI::ClickGameSettingButton);
	SelectSettingArray[1]->Button->OnClicked.AddDynamic(this, &UUserSettingUI::ClickAudioSettingButton);
	//SelectSettingArray[Graphics]->Button->OnClicked.AddDynamic(this, &UUserSettingUI::ClickGraphicsSettingButton);
	SelectSettingArray[2]->Button->OnClicked.AddDynamic(this, &UUserSettingUI::ClickKeyguideSettingButton);
	SelectSettingArray[3]->Button->OnClicked.AddDynamic(this, &UUserSettingUI::ClickQuitSettingButton);

	WBP_UserSetting_GameUI->LightSettingButton->OnClicked.AddDynamic(this, &UUserSettingUI::ClickLightSettingButton);

	WBP_UserSetting_GameUI->WBP_Language_Button->RightButton->OnClicked.AddDynamic(this, &UUserSettingUI::ChangeLanguage);
	WBP_UserSetting_GameUI->WBP_Language_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSettingUI::ChangeLanguage);
}

void UUserSettingUI::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(OpenAnimation);
	Open();
	index = 0;
}

void UUserSettingUI::NativeDestruct()
{
	Super::NativeDestruct();
	UMG_GameExit->SetVisibility(ESlateVisibility::Collapsed);
	WBP_UserSetting_LightUI->SetVisibility(ESlateVisibility::Collapsed);
}

void UUserSettingUI::ClickGameSettingButton()
{
	UnselectAllButton();
	SubUserSettingArray[Game]->SetVisibility(ESlateVisibility::Visible);
	Cast<UUserSettingGameUI>(SubUserSettingArray[Game])->SetKeyboardFocus();
	PlayAnimation(OpenGameSettingAnimation);
	WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Camera->Select();
}

void UUserSettingUI::ClickAudioSettingButton()
{
	UnselectAllButton();
	SubUserSettingArray[Audio]->SetVisibility(ESlateVisibility::Visible);
	Cast<UUserSettingAudioUI>(SubUserSettingArray[Audio])->SetKeyboardFocus();
	PlayAnimation(OpenAudioSettingAnimation);
	WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Audio->Select();
}

void UUserSettingUI::ClickGraphicsSettingButton()
{
	UnselectAllButton();
	SubUserSettingArray[Graphics]->SetVisibility(ESlateVisibility::Visible);
	Cast<UUserSetting_GraphicsUI>(SubUserSettingArray[Graphics])->SetKeyboardFocus();
	PlayAnimation(OpenGraphicsSettingAnimation);
	//WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Graphics->Select();
}

void UUserSettingUI::ClickQuitSettingButton()
{
	UMG_GameExit->SetVisibility(ESlateVisibility::Visible);
	UMG_GameExit->SetKeyboardFocus();
	//if (!GameExitUI->IsInViewport())
	//{
	//	GameExitUI->AddToViewport();
	//}
}

void UUserSettingUI::ClickLightSettingButton()
{
	WBP_UserSetting_LightUI->SetVisibility(ESlateVisibility::Visible);
}

void UUserSettingUI::ClickKeyguideSettingButton()
{
	UnselectAllButton();
	WBP_UserSetting_KeyGuideUI->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(OpenKeyGuideSettingAnimation);
	WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Keyguide->Select();
}


void UUserSettingUI::UnselectAllButton()
{
	for (int i = 0; i < SelectSettingArray.Num(); i++) {
		SelectSettingArray[i]->Unselect();
	}
	for (int i = 0; i < SubUserSettingArray.Num(); i++) {
		SubUserSettingArray[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
	WBP_UserSetting_KeyGuideUI->SetVisibility(ESlateVisibility::Collapsed);
}

void UUserSettingUI::ChangeLanguage()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	WBP_UserSetting_SelectUI->ChangeLanguage(GameInstance->language);
	WBP_UserSetting_AudioUI->ChangeLanguage(GameInstance->language);
	WBP_UserSetting_LightUI->ChangeLanguage(GameInstance->language);
	WBP_UserSetting_GraphicsUI->ChangeLanguage(GameInstance->language);
	UMG_GameExit->ChangeLanguage(GameInstance->language);
}

void UUserSettingUI::Open()
{
	this->SetVisibility(ESlateVisibility::Visible);
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	ChangeLanguage();
	Controller->SetInputMode(FInputModeUIOnly());
	Controller->bShowMouseCursor = true;
	Controller->SetPause(true);
	this->SetKeyboardFocus();
	WBP_UserSetting_SelectUI->SetFocus();
	PlayAnimation(OpenAnimation);
}

void UUserSettingUI::Close()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	AJesusPlayerController* Controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance->MainMenuWidget->IsInViewport())
	{
		Controller->SetInputMode(FInputModeGameOnly());
		Controller->bShowMouseCursor = false;
		Controller->SetPause(false);
	}
	else {
		Controller->SetInputMode(FInputModeUIOnly());
		Controller->bShowMouseCursor = true;
		Controller->SetPause(true);
	}
	this->RemoveFromParent();
}

FReply UUserSettingUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();
	if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::Platform_Delete || InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Right)
	{
		Close();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Down || InKeyEvent.GetKey() == EKeys::Gamepad_DPad_Down)
	{
		SelectSettingArray[index]->Unselect();
		
		index = FMath::Clamp(index + 1, 0, SelectSettingArray.Num() -1);
		
		SelectSettingArray[index]->OnHovered();

		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Up || InKeyEvent.GetKey() == EKeys::Gamepad_DPad_Up)
	{
		SelectSettingArray[index]->Unselect();
		index = FMath::Clamp(index - 1, 0, SelectSettingArray.Num() -1);
		SelectSettingArray[index]->OnHovered();
		Reply = FReply::Handled();
	}
	
	if (InKeyEvent.GetKey() == EKeys::Enter || InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
	{
		SelectSettingArray[index]->Select();
		SelectSettingArray[index]->Button->OnClicked.Broadcast();
		Reply = FReply::Handled();
	}

	return Reply;
}





