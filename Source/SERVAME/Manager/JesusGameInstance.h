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

UENUM()
enum class Language :uint8
{
	ENG		UMETA(DisplayName = "English"),
	KOR		UMETA(DisplayName = "Korean")
};

UCLASS()
class SERVAME_API UJesusGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
		UJesusGameInstance();
		void InitInstance();
		void InitDefaultSetting();
		virtual void Init();

		void SetPostProcessBrightness(float value);
		float GetPostProcessBrightness();
		void SetLanguage(Language value);

		FStartSetting PlayerOptionSetting;

		UPROPERTY()
		TSubclassOf<UMainMenuUI> MainMenuWidgetClass;

		UPROPERTY()
		UMainMenuUI* MainMenuWidget;

		UPROPERTY()
		TSubclassOf<UDebugLogWidget> DebugLogWidgetClass;

		UPROPERTY()
		UDebugLogWidget* DebugLogWidget;

		UPROPERTY()
		Language language;
};
