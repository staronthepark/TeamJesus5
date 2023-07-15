// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "TriggerActor.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API ATriggerActor : public ABaseTriggerActor
{
	GENERATED_BODY()

public:
	ATriggerActor();


	virtual void BeginPlay();
	virtual void BeginTriggerEvent() override;
	virtual void EndTriggerEvent() override;
	virtual void EnableEvent() override;
};
