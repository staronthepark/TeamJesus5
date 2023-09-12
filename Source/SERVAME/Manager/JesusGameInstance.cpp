// Fill out your copyright notice in the Description page of Project Settings.


#include "JesusGameInstance.h"
#include "Engine/PostProcessVolume.h"
#include "GameFramework/GameUserSettings.h"
#include "SoundManager.h"
#include "../JesusSaveGame.h"
#include "../BaseTriggerActor.h"
#include "Kismet/GameplayStatics.h"

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

		setting->SetPostProcessingQuality(3);
		setting->SetShadowQuality(3);
		setting->SetGlobalIlluminationQuality(3);
		setting->SetVisualEffectQuality(3);


		setting->SetReflectionQuality(3);
		setting->SetTextureQuality(3);
		setting->SetFoliageQuality(3);
		setting->SetShadingQuality(3);
		setting->SetViewDistanceQuality(3);
		setting->SetAntiAliasingQuality(3);
		
		setting->ApplySettings(true);
	}

	TArray<AActor*> ActorsToFind;
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseTriggerActor::StaticClass(), ActorsToFind);
	}
	for (AActor* TriggerActor : ActorsToFind)
	{
		ABaseTriggerActor* TriggerActorCast = Cast<ABaseTriggerActor>(TriggerActor);
		if (TriggerActorCast)
		{

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

