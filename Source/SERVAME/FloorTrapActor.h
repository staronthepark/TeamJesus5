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


		FTimerHandle Timer;

		UPROPERTY(EditAnyWhere)
			bool IsTimer;
		UPROPERTY(EditAnywhere)
			float Damage;
		UPROPERTY(EditAnywhere)
			float Speed;
		UPROPERTY(EditAnywhere)
			float Time;

		UPROPERTY(EditAnywhere)
			UStaticMeshComponent* MeshComp;
		UPROPERTY(EditAnywhere)
			USceneComponent* SceneComp;
		
private:

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void BeginTriggerEvent()override;
	virtual void EndTriggerEvent()override;
	virtual void EnableEvent()override;

	void EnableTrap();
};
