// Fill out your copyright notice in the Description page of Project Settings.


#include "JesusGameInstance.h"
#include "Engine/PostProcessVolume.h"
#include "GameFramework/GameUserSettings.h"
#include "SoundManager.h"
#include "../JesusSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include <SERVAME/UI/PlayerSoulStatUI.h>


#define LOW 0
#define MIDDLE 1
#define HIGH 2
#define VERYHIGH 3

UJesusGameInstance::UJesusGameInstance()
{
	static ConstructorHelpers::FClassFinder<UDebugLogWidget> DLW(TEXT("/Game/02_Resource/04_UI/01_WBP/99_Debug/WBP_DebugLog"));
	static ConstructorHelpers::FClassFinder<UMainMenuUI> ASD(TEXT("/Game/02_Resource/04_UI/01_WBP/00_MainMenu/WBP_MainMenu"));

	if (DLW.Succeeded())
		DebugLogWidgetClass = DLW.Class;
	MainMenuWidgetClass = ASD.Class;
}

void UJesusGameInstance::InitInstance()
{
	if (IsValid(DebugLogWidgetClass))
	{
		DebugLogWidget = Cast<UDebugLogWidget>(CreateWidget(GetWorld(), DebugLogWidgetClass));
	}

	MainMenuWidget = Cast<UMainMenuUI>(CreateWidget(GetWorld(), MainMenuWidgetClass));

	language = Language::KOR;
}

void UJesusGameInstance::InitDefaultSetting()
{
	PlayerOptionSetting.Gamma = 0.2f;
	PlayerOptionSetting.DPI = 40.0f;

	UWorld* World = GetWorld();


	for (auto Actor : World->PostProcessVolumes)
	{
		if (Actor->GetProperties().bIsUnbound) {
			FPostProcessSettings* PostProcessSettings = (FPostProcessSettings*)Actor->GetProperties().Settings;
			PostProcessSettings->AutoExposureMaxBrightness = PlayerOptionSetting.Gamma;
			PostProcessSettings->AutoExposureMinBrightness = PlayerOptionSetting.Gamma;
			break;
		}
	}
}

void UJesusGameInstance::Init()
{
	UGameUserSettings* setting = GEngine->GetGameUserSettings();
	if (setting)
	{
		//setting->RunHardwareBenchmark();
		//
		//float GPU = setting->GetLastGPUBenchmarkResult() / 190;
		//
		//UE_LOG(LogTemp, Error, TEXT("%f"), GPU);
		
		setting->SetFrameRateLimit(60);
		setting->SetVSyncEnabled(false);
		
		setting->SetResolutionScaleValue(100);

		setting->SetPostProcessingQuality(2);
		setting->SetShadowQuality(2);
		setting->SetGlobalIlluminationQuality(2);
		setting->SetVisualEffectQuality(2);


		setting->SetReflectionQuality(2);
		setting->SetTextureQuality(2);
		setting->SetFoliageQuality(2);
		setting->SetShadingQuality(2);
		setting->SetViewDistanceQuality(2);
		setting->SetAntiAliasingQuality(2);
		
		setting->ApplySettings(true);
	}	
}

void UJesusGameInstance::SetPostProcessBrightness(float value)
{
	PlayerOptionSetting.Gamma = value;
	for (auto Actor : GetWorld()->PostProcessVolumes) {
		if (Actor->GetProperties().bIsUnbound) {
			FPostProcessSettings* PostProcessSettings = (FPostProcessSettings*)Actor->GetProperties().Settings;
			PostProcessSettings->AutoExposureMaxBrightness = PlayerOptionSetting.Gamma;
			PostProcessSettings->AutoExposureMinBrightness = PlayerOptionSetting.Gamma;
			break;
		}
	}
}

float UJesusGameInstance::GetPostProcessBrightness()
{
	return PlayerOptionSetting.Gamma;
}

void UJesusGameInstance::SetLanguage(Language value)
{
	language = value;
}

