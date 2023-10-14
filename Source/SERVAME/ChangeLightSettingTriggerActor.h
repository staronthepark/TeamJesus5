// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerActor.h"
#include "./LevelLightingManager.h"
#include "ChangeLightSettingTriggerActor.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API AChangeLightSettingTriggerActor : public ATriggerActor
{
	GENERATED_BODY()

public:
	AChangeLightSettingTriggerActor();

		UPROPERTY(Editanywhere)
			FString MapName;

		ALevelLightingManager* LightManager;

		virtual void BeginPlay();
		virtual void BeginTriggerEvent() override;
		virtual void EndTriggerEvent() override;
		virtual void EnableEvent() override;
};
