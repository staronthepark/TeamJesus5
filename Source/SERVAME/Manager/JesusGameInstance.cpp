// Fill out your copyright notice in the Description page of Project Settings.


#include "JesusGameInstance.h"
#include "Engine/PostProcessVolume.h"
#include "GameFramework/GameUserSettings.h"
#include "SoundManager.h"
#include "Kismet/GameplayStatics.h"

UJesusGameInstance::UJesusGameInstance()
{


	static ConstructorHelpers::FClassFinder<UDebugLogWidget> DLW(TEXT("/Game/02_Resource/04_UI/01_WBP/99_Debug/WBP_DebugLog"));


	static ConstructorHelpers::FClassFinder<UMainMenuUI> ASD(TEXT("/Game/02_Resource/04_UI/01_WBP/00_MainMenu/WBP_MainMenu"));

	if (DLW.Succeeded())
		DebugLogWidgetClass = DLW.Class;
	MainMenuWidgetClass = ASD.Class;
}
//#include "HardwareInfo.h"
//#include "GenericPlatform/GenericPlatformMisc.h"
void UJesusGameInstance::InitInstance()
{
	if (IsValid(DebugLogWidgetClass))
	{
		DebugLogWidget = Cast<UDebugLogWidget>(CreateWidget(GetWorld(), DebugLogWidgetClass));
	}

	//FGenericPlatformMisc::benchmark();

	MainMenuWidget = Cast<UMainMenuUI>(CreateWidget(GetWorld(), MainMenuWidgetClass));

	ASoundManager::GetInstance().Init();
	ASoundManager::GetInstance().StartBGMSound();

	
	//MainMenuWidget->AddToViewport();
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
		setting->RunHardwareBenchmark();

		UE_LOG(LogTemp, Error, TEXT("%f"), setting->GetLastGPUBenchmarkResult());
		//setting->SetFrameRateLimit(60);
		//setting->SetVSyncEnabled(false);
		//setting->SetPostProcessingQuality(1);
		//setting->SetShadowQuality(1);
		//setting->SetGlobalIlluminationQuality(1);
		//setting->SetReflectionQuality(2);
		//setting->SetVisualEffectQuality(2);
		//
		////const
		//setting->SetTextureQuality(0);
		//setting->SetFoliageQuality(0);
		//setting->SetShadingQuality(0);
		//setting->SetViewDistanceQuality(0);
		//setting->SetAntiAliasingQuality(1);
		//setting->SetResolutionScaleValue(0);
		//
		//setting->ApplySettings(true);
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

