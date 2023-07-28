// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Components/WidgetComponent.h"
#include "..\UI\DebugLogWidget.h"
#include "..\Player\PlayerAnimInstance.h"
#include "..\UI\MainMenuUI.h"
#include <Engine/PostProcessVolume.h>

#include "JesusGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FStartSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Bleeding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Gamma;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DPI;
};

UCLASS()
class SERVAME_API UJesusGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
		UJesusGameInstance();
		void InitInstance();
		void InitDefaultSetting();

		void SetPostProcessBrightness(float value);
		float GetPostProcessBrightness();

		FStartSetting PlayerOptionSetting;

		UPROPERTY()
			TSubclassOf<UMainMenuUI> MainMenuWidgetClass;

		UPROPERTY()
			UMainMenuUI* MainMenuWidget;

		UPROPERTY()
		TSubclassOf<UDebugLogWidget> DebugLogWidgetClass;

		UPROPERTY()
		UDebugLogWidget* DebugLogWidget;
};
