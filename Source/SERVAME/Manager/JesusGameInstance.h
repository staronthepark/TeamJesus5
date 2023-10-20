// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Components/WidgetComponent.h"
#include "..\UI\DebugLogWidget.h"
#include "..\UI\MainMenuUI.h"
#include <Engine/PostProcessVolume.h>
#include "../BaseTriggerActor.h"
#include "JesusGameInstance.generated.h"

class UPlayerSoulStatUI;

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

		TMap<int32, ABaseTriggerActor*>SavedTriggerActor;


		float GetPostProcessBrightness();

		void InitInstance();
		void InitDefaultSetting();
		void SetLanguage(Language value);
		void SetPostProcessBrightness(float value);
		//void SavePlayerInfo(FVector Location, FRotator Rotation);

		virtual void Init();
};
