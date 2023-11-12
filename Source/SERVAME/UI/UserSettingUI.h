// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserSettingSelectUI.h"
#include "UserSettingGameUI.h"
#include "UserSetting_GraphicsUI.h"
#include "SubUserSettingUI.h"
#include "UserSettingAudioUI.h"
#include "UserSettingKeyGuideUI.h"
#include "GameExitUI.h"

#include "UserSettingUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UUserSettingUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UUserSettingSelectUI* WBP_UserSetting_SelectUI;

	UPROPERTY(meta = (BindWidget))
		UUserSettingAudioUI* WBP_UserSetting_AudioUI;

	UPROPERTY(meta = (BindWidget))
		UUserSetting_GraphicsUI* WBP_UserSetting_GraphicsUI;

	UPROPERTY(meta = (BindWidget))
		UUserSettingKeyGuideUI* WBP_UserSetting_KeyGuideUI;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* OpenAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* OpenGameSettingAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* OpenAudioSettingAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* OpenGraphicsSettingAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* OpenKeyGuideSettingAnimation;

	TArray<USelectTextUI*> SelectSettingArray;
	TArray<USubUserSettingUI*> SubUserSettingArray;


	UPROPERTY(meta = (BindWidget))
		UUserSettingLightUI* WBP_UserSetting_LightUI;

	UPROPERTY(meta = (BindWidget))
		UGameExitUI* UMG_GameExit;
	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UGameExitUI> GameExitUIClass;
	//UGameExitUI* GameExitUI;

	int index;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	UUserSettingGameUI* WBP_UserSetting_GameUI;


	UFUNCTION()
	void ClickGameSettingButton();

	UFUNCTION()
	void ClickAudioSettingButton();

	UFUNCTION()
	void ClickGraphicsSettingButton();

	UFUNCTION()
	void ClickQuitSettingButton();

	UFUNCTION()
	void ClickLightSettingButton();

	UFUNCTION()
	void ClickKeyguideSettingButton();

	void UnselectAllButton();

	UFUNCTION()
	void ChangeLanguage();

	UFUNCTION()
	void Open();

	UFUNCTION()
	void Close();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
};
