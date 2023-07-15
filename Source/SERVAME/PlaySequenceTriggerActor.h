// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerActor.h"
#include "./Boss_1/JesusBoss.h"
#include "./Player/PlayerCharacter.h"
#include "CineCameraActor.h"
#include "PlaySequenceTriggerActor.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API APlaySequenceTriggerActor : public ATriggerActor
{
	GENERATED_BODY()

public:
	APlaySequenceTriggerActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool KeepActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TriggerOnlyOnce;

	UPROPERTY(EditAnywhere)
		UBoxComponent* Collision;

	ALevelSequenceActor* SequenceActor;
	UPROPERTY(EditAnywhere)
		ULevelSequence* Sequence;

	AJesusBoss* Boss;

	ACineCameraActor* CineCameraActor;

	UPROPERTY()
		ULevelSequencePlayer* SequncePlayer;

	
	APlayerCharacter* Character;

	virtual void BeginPlay() override;
	virtual void BeginTriggerEvent() override;
	virtual void EndTriggerEvent() override;
	virtual void EnableEvent() override;

	UFUNCTION()
	void EndSequence();
};
