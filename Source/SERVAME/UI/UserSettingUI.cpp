// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettingUI.h"
#include "..\Manager\SoundManager.h"
#include <SERVAME/Manager/JesusGameInstance.h>
#include <SERVAME/Player/JesusPlayerController.h>

#define Game 0
#define Audio 1
#define Graphics 2
#define Quit 3


void UUserSettingUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//GameExitUI = Cast<UGameExitUI>(CreateWidget(GetWorld(), GameExitUIClass));
	SelectSettingArray.Add(WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Camera); //0
	SelectSettingArray.Add(WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Audio); //1
	SelectSettingArray.Add(WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Graphics); //2
	SelectSettingArray.Add(WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Quit); //3

	SubUserSettingArray.Add(WBP_UserSetting_GameUI); // 0
	SubUserSettingArray.Add(WBP_UserSetting_AudioUI); // 1
	SubUserSettingArray.Add(WBP_UserSetting_GraphicsUI); //2

	SelectSettingArray[Game]->Button->OnClicked.AddDynamic(this, &UUserSettingUI::ClickGameSettingButton);
	SelectSettingArray[Audio]->Button->OnClicked.AddDynamic(this, &UUserSettingUI::ClickAudioSettingButton);
	SelectSettingArray[Graphics]->Button->OnClicked.AddDynamic(this, &UUserSettingUI::ClickGraphicsSettingButton);
	SelectSettingArray[Quit]->Button->OnClicked.AddDynamic(this, &UUserSettingUI::ClickQuitSettingButton);

	WBP_UserSetting_GameUI->LightSettingButton->OnClicked.AddDynamic(this, &UUserSettingUI::ClickLightSettingButton);

	WBP_UserSetting_GameUI->WBP_Language_Button->RightButton->OnClicked.AddDynamic(this, &UUserSettingUI::ChangeLanguage);
	WBP_UserSetting_GameUI->WBP_Language_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSettingUI::ChangeLanguage);
}

void UUserSettingUI::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(OpenAnimation);
	Open();
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
	PlayAnimation(OpenGameSettingAnimation);
	WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Camera->Select();
}

void UUserSettingUI::ClickAudioSettingButton()
{
	UnselectAllButton();
	SubUserSettingArray[Audio]->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(OpenAudioSettingAnimation);
	WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Audio->Select();
}

void UUserSettingUI::ClickGraphicsSettingButton()
{
	UnselectAllButton();
	SubUserSettingArray[Graphics]->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(OpenGraphicsSettingAnimation);
	WBP_UserSetting_SelectUI->WBP_Setting_SelectText_Graphics->Select();
}

void UUserSettingUI::ClickQuitSettingButton()
{
	UMG_GameExit->SetVisibility(ESlateVisibility::Visible);
	//if (!GameExitUI->IsInViewport())
	//{
	//	GameExitUI->AddToViewport();
	//}
}

void UUserSettingUI::ClickLightSettingButton()
{
	WBP_UserSetting_LightUI->SetVisibility(ESlateVisibility::Visible);
}


void UUserSettingUI::UnselectAllButton()
{
	for (int i = 0; i < SelectSettingArray.Num(); i++) {
		SelectSettingArray[i]->Unselect();
	}
	for (int i = 0; i < SubUserSettingArray.Num(); i++) {
		SubUserSettingArray[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUserSettingUI::ChangeLanguage()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	WBP_UserSetting_SelectUI->ChangeLanguage();
	WBP_UserSetting_AudioUI->ChangeLanguage();
	WBP_UserSetting_LightUI->ChangeLanguage();
	WBP_UserSetting_GraphicsUI->ChangeLanguage();
	UMG_GameExit->ChangeLanguage();
}

void UUserSettingUI::Open()
{
	this->SetVisibility(ESlateVisibility::Visible);
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
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
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		Close();
		Reply = FReply::Handled();
	}
	return Reply;
}





