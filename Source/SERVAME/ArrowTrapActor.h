// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "FloorTrapActor.h"
#include "ArrowTrapActor.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API AArrowTrapActor : public AFloorTrapActor
{
	GENERATED_BODY()
	
public:
	AArrowTrapActor();

	UPROPERTY(Editanywhere)
		USceneComponent* SceneComp1;
	UPROPERTY(Editanywhere)
		USceneComponent* SceneComp2;
	UPROPERTY(Editanywhere)
		USceneComponent* SceneComp3;

protected:

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void BeginTriggerEvent()override;
	virtual void EndTriggerEvent()override;
	virtual void EnableEvent()override;
	virtual void EnableTrap() override;

};
