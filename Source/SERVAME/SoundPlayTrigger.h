// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerActor.h"
#include "Components/AudioComponent.h"
#include "SoundPlayTrigger.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API ASoundPlayTrigger : public ATriggerActor
{
	GENERATED_BODY()

	ASoundPlayTrigger();


public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAudioComponent* AudioComp;

	void PlayAudio();

	virtual void BeginPlay() override;
	virtual void BeginTriggerEvent() override;
	virtual void EndTriggerEvent() override;
	virtual void EnableEvent() override;
};
