// Fill out your copyright notice in the Description page of Project Settings.


#include "JesusGameInstance.h"
#include "Engine/PostProcessVolume.h"
#include "SoundManager.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

UJesusGameInstance::UJesusGameInstance()
{


	static ConstructorHelpers::FClassFinder<UDebugLogWidget> DLW(TEXT("/Game/02_Resource/04_UI/01_WBP/99_Debug/WBP_DebugLog"));


	static ConstructorHelpers::FClassFinder<UMainMenuUI> ASD(TEXT("/Game/02_Resource/04_UI/01_WBP/00_MainMenu/WBP_MainMenu"));

	if (DLW.Succeeded())
		DebugLogWidgetClass = DLW.Class;
	MainMenuWidgetClass = ASD.Class;
}

#define LOW 0
#define MIDDLE 1
#define HIGH 2
#define VERYHIGH 3

void UJesusGameInstance::Init()
{
	Super::Init();
	int32 scalabilityLevel = 0;
	//get game user setting
	UGameUserSettings* setting = GEngine->GetGameUserSettings();
	if (setting)
	{
		setting->SetPostProcessingQuality(MIDDLE);
		setting->SetShadowQuality(MIDDLE);
		setting->SetGlobalIlluminationQuality(MIDDLE);
		setting->SetReflectionQuality(HIGH);
		setting->SetVisualEffectQuality(HIGH);

		//const
		setting->SetFoliageQuality(LOW);
		setting->SetShadingQuality(LOW);
		setting->SetViewDistanceQuality(LOW);
		setting->SetAntiAliasingQuality(LOW);
		setting->SetTextureQuality(LOW);
		setting->SetResolutionScaleValue(0);

		setting->ApplySettings(true);
	}

	ASoundManager::GetInstance().Init();
	ASoundManager::GetInstance().StartBGMSound();

	
}

void UJesusGameInstance::InitInstance()
{
	if (IsValid(DebugLogWidgetClass))
	{
		DebugLogWidget = Cast<UDebugLogWidget>(CreateWidget(GetWorld(), DebugLogWidgetClass));
	}

	MainMenuWidget = Cast<UMainMenuUI>(CreateWidget(GetWorld(), MainMenuWidgetClass));
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

