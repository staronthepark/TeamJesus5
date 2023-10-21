// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerRestUI.h"
#include "PlayerStatUI.h"
#include "PlayerSoulStatUI.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UPlayerSoulStatUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UPlayerRestUI* WBP_PlayerRest_UI;

	UPROPERTY(meta = (BindWidget))
		UPlayerStatUI* WBP_Player_Stat_UI;
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
		void ClickPray();

	UFUNCTION()
		void OpenStat();

	void InitStat(int str, int stamina, int hp, int shield);

	void Open();

	void Close();

	void SetRemainSoul(int value);

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
};
