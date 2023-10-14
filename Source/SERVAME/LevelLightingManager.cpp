#include "LevelLightingManager.h"

ALevelLightingManager::ALevelLightingManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALevelLightingManager::BeginPlay()
{
	Super::BeginPlay();

	DirectionalLightComp
		= Cast<UDirectionalLightComponent>(DirectionalLight->GetComponentByClass(UDirectionalLightComponent::StaticClass()));
	
	ExponentialHeightFogComp
		= Cast<UExponentialHeightFogComponent>(ExponentialHeightFog->GetComponentByClass(UExponentialHeightFogComponent::StaticClass()));
	
	SkyLightComp
		= Cast<USkyLightComponent>(SkyLight->GetComponentByClass(USkyLightComponent::StaticClass()));

	for (int32 i = 0; i < LightSetting.Num(); i++)
	{
		LightSettingMap.Add(LightSetting[i].MapName, LightSetting[i]);
	}
}

void ALevelLightingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DirectionalLightComp->SetIntensity(FMath::Lerp(DirectionalLightComp->Intensity, TargetDirectionalLight.Intensity, FadeSpeed * DeltaTime));
	DirectionalLightComp->SetLightColor(FMath::Lerp(DirectionalLightComp->GetLightColor(), TargetDirectionalLight.LightColor, FadeSpeed * DeltaTime));
	
	ExponentialHeightFogComp->SetFogDensity(FMath::Lerp(ExponentialHeightFogComp->FogDensity, TargetHeightFog.FogDensity, FadeSpeed * DeltaTime));
	ExponentialHeightFogComp->SetFogHeightFalloff(FMath::Lerp(ExponentialHeightFogComp->FogHeightFalloff, TargetHeightFog.FogHeightFallOff, FadeSpeed * DeltaTime));
	ExponentialHeightFogComp->SetVolumetricFogScatteringDistribution(FMath::Lerp(ExponentialHeightFogComp->VolumetricFogScatteringDistribution, TargetHeightFog.ScatteringDistribution, FadeSpeed * DeltaTime));
	ExponentialHeightFogComp->SetVolumetricFogEmissive(FMath::Lerp(ExponentialHeightFogComp->VolumetricFogEmissive, TargetHeightFog.Emissive, FadeSpeed * DeltaTime));

	float Red   = FMath::Lerp(ExponentialHeightFogComp->VolumetricFogAlbedo.R, TargetHeightFog.Albedo.R, FadeSpeed * DeltaTime);
	float Green = FMath::Lerp(ExponentialHeightFogComp->VolumetricFogAlbedo.G, TargetHeightFog.Albedo.G, FadeSpeed * DeltaTime);
	float Blue  = FMath::Lerp(ExponentialHeightFogComp->VolumetricFogAlbedo.B, TargetHeightFog.Albedo.B, FadeSpeed * DeltaTime);

	ExponentialHeightFogComp->SetVolumetricFogAlbedo(FColor(Red, Green, Blue));
	
	SkyLightComp->SetIntensity(FMath::Lerp(SkyLightComp->Intensity, TargetSkyLight.IntensityScale, FadeSpeed * DeltaTime));
	SkyLightComp->SetLightColor(FMath::Lerp(SkyLightComp->GetLightColor(), TargetSkyLight.LightColor, FadeSpeed * DeltaTime));
}

void ALevelLightingManager::ChangeTargetLightSetting(FString Name)
{
	TargetDirectionalLight = LightSettingMap[Name].DirectionalLight;
	TargetHeightFog = LightSettingMap[Name].HeightFog;
	TargetSkyLight = LightSettingMap[Name].SkyLight;
}