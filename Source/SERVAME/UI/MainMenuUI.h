// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "UserSettingUI.h"
#include "Kismet/GameplayStatics.h"

#include "MainMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UMainMenuUI : public UUserWidget
{
	GENERATED_BODY()
		UPROPERTY(meta = (BindWidget))
		UImage* RenderTargetImage;

	UPROPERTY(meta = (BindWidget))
		UImage* StartBackgroundImage;

	//UPROPERTY(meta = (BindWidget))
	//	UButton* SettingButton;
	//UPROPERTY(meta = (BindWidget))
	//	UImage* SettingBackgroundImage;


	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;
	UPROPERTY(meta = (BindWidget))
		UImage* QuitBackgroundImage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* MainMenuCloseAnimation;

	ALevelSequenceActor* LevelSequenceActor;
	UPROPERTY(EditAnywhere)
		ULevelSequence* LevelSequence;

	UPROPERTY(meta = (BindWidget))
		UUserSettingUI* WBP_UserSettingUI;

	UPROPERTY(meta = (BindWidget))
		UGameExitUI* UMG_GameExit;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UUserSettingUI> UserSettingUIClass;
	//UUserSettingUI* UserSettingUI;

public:
	ULevelSequencePlayer* LevelSequencePlayer;

	UPROPERTY(meta = (BindWidget))
		UButton* StartButton;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
		void HoverStartButton();
	//UFUNCTION()
	//	void HoverSettingButton();
	UFUNCTION()
		void HoverQuitButton();

	UFUNCTION()
		void UnhoverStartButton();
	//UFUNCTION()
	//	void UnhoverSettingButton();
	UFUNCTION()
		void UnhoverQuitButton();


	UFUNCTION()
	void ClickStartButton();
	//UFUNCTION()
	//void ClickSettingButton();
	UFUNCTION()
	void ClickQuitButton();

	UFUNCTION()
	void SequenceFinish();
};
