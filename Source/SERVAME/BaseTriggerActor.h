// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxTriggerComp.h"
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

	//TArray<UParticleSystemComponent*>ParticleArray;

public:	

	ABaseTriggerActor();

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginTriggerEvent();
	virtual void EndTriggerEvent();
	virtual void EnableEvent();

};
