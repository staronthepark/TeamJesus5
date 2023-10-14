// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "FloorTrapActor.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API AFloorTrapActor : public AInteractionActor
{
	GENERATED_BODY()

public:
		AFloorTrapActor();



		UPROPERTY(EditAnyWhere)
			bool IsTimer;
		UPROPERTY(EditAnywhere)
			UStaticMeshComponent* MeshComp;
private:


	virtual void BeginPlay() override;
	virtual void BeginTriggerEvent()override;
	virtual void EndTriggerEvent()override;
	virtual void EnableEvent()override;
};
