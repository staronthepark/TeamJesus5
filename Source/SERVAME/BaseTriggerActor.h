// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxTriggerComp.h"
#include "Particles/ParticleSystemComponent.h"
#include "BaseTriggerActor.generated.h"

UCLASS()
class SERVAME_API ABaseTriggerActor : public AActor
{
	GENERATED_BODY()
	
protected:

	bool IsOverlap;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AutoActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxTriggerComp* TriggerComp;

	TArray<UParticleSystemComponent*>ParticleArray;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsActive;

	ABaseTriggerActor();

	virtual void Load(bool value);
	virtual void Init();
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginTriggerEvent();
	virtual void EndTriggerEvent();
	virtual void EnableEvent();

};
