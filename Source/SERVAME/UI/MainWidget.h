// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	//UPROPERTY(Transient, meta = (BindWidgetAnim))
	//UWidgetAnimation* IntroAnimation;

	FString LevelToLoad;
	UFUNCTION()
	void StartIntroAnimation();

	UFUNCTION(BlueprintCallable)
	void StoppedIntroAnimation();
	
	bool LevelLoaded;

	FWidgetAnimationDynamicEvent StartIntroAnimationDelegate;
	FWidgetAnimationDynamicEvent StopIntroAnimationDelegate;

};
