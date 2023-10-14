#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyLightComponent.h"
#include "LevelLightingManager.generated.h"

USTRUCT(BlueprintType)
struct FMyDirectionalLight
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Intensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor LightColor;
};

USTRUCT(BlueprintType)
struct FMYExponentialHeightFog
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExponentialHeightFog")
		float FogDensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExponentialHeightFog")
		float FogHeightFallOff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Volumetric")
		float ScatteringDistribution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Volumetric")
		FColor Albedo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Volumetric")
		FLinearColor Emissive;
};

USTRUCT(BlueprintType)
struct FMySkyLight
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float IntensityScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor LightColor;
};

USTRUCT(BlueprintType)
struct FLightSetting
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString MapName;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMyDirectionalLight DirectionalLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMYExponentialHeightFog HeightFog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMySkyLight SkyLight;
};

UCLASS()
class SERVAME_API ALevelLightingManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelLightingManager();
		
	UPROPERTY(Editanywhere)
		float FadeSpeed;

	UPROPERTY(Editanywhere)
		TArray<FLightSetting> LightSetting;

	TMap<FString, FLightSetting>LightSettingMap;

	FMyDirectionalLight TargetDirectionalLight;
	FMYExponentialHeightFog TargetHeightFog;
	FMySkyLight TargetSkyLight;

	ADirectionalLight* DirectionalLight;
	AExponentialHeightFog* ExponentialHeightFog;
	ASkyLight* SkyLight;

	UDirectionalLightComponent* DirectionalLightComp;
	UExponentialHeightFogComponent* ExponentialHeightFogComp;
	USkyLightComponent* SkyLightComp;


protected:

public:	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void ChangeTargetLightSetting(FString Name);
};