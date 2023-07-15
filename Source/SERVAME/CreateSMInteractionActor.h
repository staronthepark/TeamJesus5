// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "CreateSMInteractionActor.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API ACreateSMInteractionActor : public ABaseTriggerActor
{
	GENERATED_BODY()
	
public:
	ACreateSMInteractionActor();

	TArray<UStaticMeshComponent*> MeshCompArray;
	//TArray<UParticleSystemComponent*> ParticleCompArray;

public:

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void BeginTriggerEvent();
	virtual void EndTriggerEvent();
	virtual void EnableEvent();
};
