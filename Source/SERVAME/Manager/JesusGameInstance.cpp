// Fill out your copyright notice in the Description page of Project Settings.


#include "JesusGameInstance.h"
#include "Engine/PostProcessVolume.h"
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

void UJesusGameInstance::InitInstance()
{
	ASoundManager::GetInstance().Init();
	ASoundManager::GetInstance().StartBGMSound();

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

	//TArray<AActor*> PostProcessVolumes;
	//UGameplayStatics::GetAllActorsOfClass(World, APostProcessVolume::StaticClass(), PostProcessVolumes);

	for (auto Actor : World->PostProcessVolumes)
	{
		if (Actor->GetProperties().bIsUnbound) {
			FPostProcessSettings* PostProcessSettings = (FPostProcessSettings*)Actor->GetProperties().Settings;
			PostProcessSettings->AutoExposureMaxBrightness = PlayerOptionSetting.Gamma;
			PostProcessSettings->AutoExposureMinBrightness = PlayerOptionSetting.Gamma;
			break;
		}
		//FPostProcessSettings PostProcessSettings = PostProcessVolume->Settings;

		//PostProcessSettings.bOverride_SceneColorTint = true;
		//PostProcessSettings.SceneColorTint = FLinearColor(PlayerOptionSetting.Gamma, PlayerOptionSetting.Gamma, PlayerOptionSetting.Gamma, 1.0f);
		//PostProcessSettings.BloomIntensity = PlayerOptionSetting.Gamma;
		//PostProcessVolume->Settings = PostProcessSettings;

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

