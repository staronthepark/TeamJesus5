// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include ".\Manager\JesusGameInstance.h"
#include "Components/PointLightComponent.h"
#include "SavePointInteraction.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API ASavePointInteraction : public AInteractionActor
{
	GENERATED_BODY()
	

public:
	ASavePointInteraction();

	bool IsActive;

	float DissolveValue;

	TArray<UStaticMeshComponent*>MeshCompArray;
	//TArray<UParticleSystemComponent*>ParticleCompArray;

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);


	virtual void BeginTriggerEvent();
	virtual void EndTriggerEvent();
	virtual void EnableEvent();
};