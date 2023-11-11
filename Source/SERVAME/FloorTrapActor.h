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

		float ButtonOriginPosZ;

		UPROPERTY(EditAnyWhere)
			bool IsTimer;
		UPROPERTY(EditAnywhere)
			float Damage;
		UPROPERTY(EditAnywhere)
			float Speed;
		UPROPERTY(EditAnywhere)
			float Time;

		UPROPERTY(EditAnywhere)
			USceneComponent* SceneComp;

		UPROPERTY(EditAnywhere)
			UStaticMeshComponent* ButtonMeshComp;

		UPROPERTY(EditAnywhere)
			UBoxComponent* BoxComp;

		bool IsTrigger;
		bool IsEnable;
		bool OnTheButton;
		
private:

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* MeshComp;

protected:

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void BeginTriggerEvent()override;
	virtual void EndTriggerEvent()override;
	virtual void EnableEvent()override;

	virtual void EnableTrap();

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
