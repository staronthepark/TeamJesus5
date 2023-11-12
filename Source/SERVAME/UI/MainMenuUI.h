// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "UserSettingUI.h"
#include "Kismet/GameplayStatics.h"

#include "MainMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UMainMenuUI: public UUserWidget
{
	GENERATED_BODY()
		//UPROPERTY(meta = (BindWidget))
		//UImage* RenderTargetImage;

	UPROPERTY(meta = (BindWidget))
		UImage* StartBackgroundImage;

	UPROPERTY(meta = (BindWidget))
		UImage* ContinueBackgroundImage;

	UPROPERTY(meta = (BindWidget))
		UImage* OptionBackgroundImage;

	UPROPERTY(meta = (BindWidget))
		UImage* CreditBackgroundImage;

	UPROPERTY(meta = (BindWidget))
		UButton* OptionButton;

	//UPROPERTY(meta = (BindWidget))
	//	UButton* SettingButton;
	//UPROPERTY(meta = (BindWidget))
	//	UImage* SettingBackgroundImage;


	UPROPERTY(meta = (BindWidget))
		UButton* CreditButton;

	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;
	UPROPERTY(meta = (BindWidget))
		UImage* QuitBackgroundImage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* FadeOutAnimation;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* MainMenuCloseAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* FadeInAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* FadeOutForNewGame;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* CreditAnimation;

	FWidgetAnimationDynamicEvent EndFadeInDelegate;
	FWidgetAnimationDynamicEvent EndFadeOutDelegate;

	//ALevelSequenceActor* LevelSequenceActor;
	//UPROPERTY(EditAnywhere)
	//	ULevelSequence* LevelSequence;

	//UPROPERTY(meta = (BindWidget))
	//	UUserSettingUI* WBP_UserSettingUI;

	UPROPERTY(meta = (BindWidget))
		UGameExitUI* UMG_GameExit;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UUserSettingUI> UserSettingUIClass;
	//UUserSettingUI* UserSettingUI;

public:
	/*ULevelSequencePlayer* LevelSequencePlayer;*/

	UPROPERTY(meta = (BindWidget))
		UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
		UButton* ContinueButton;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
		void HoverStartButton();
	//UFUNCTION()
	//	void HoverSettingButton();

	UFUNCTION()
		void HoverContinueButton();

	UFUNCTION()
		void HoverOptionButton();

	UFUNCTION()
		void HoverCreditButton();
	
	UFUNCTION()
		void HoverQuitButton();

	UFUNCTION()
		void UnhoverStartButton();

	UFUNCTION()
		void UnhoverContinueButton();
	UFUNCTION()
		void UnhoverOptionButton();
	//UFUNCTION()
	//	void UnhoverSettingButton();

	UFUNCTION()
		void UnhoverCreditButton();
	UFUNCTION()
		void UnhoverQuitButton();


	UFUNCTION()
	void ClickStartButton();
	//UFUNCTION()
	//void ClickSettingButton();

	UFUNCTION()
	void ClickContinueButton();

	UFUNCTION()
	void ClickOptionButton();

	UFUNCTION()
	void ClickCreditButton();

	UFUNCTION()
	void ClickQuitButton();

	//UFUNCTION()
	//void SequenceFinish();

	UFUNCTION()
	void PlayFadeOutAnimation();

	UFUNCTION()
		void OnEndFadeOut();
};
