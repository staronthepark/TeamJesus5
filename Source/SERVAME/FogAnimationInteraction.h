// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimationInteraction.h"
#include "FogAnimationInteraction.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API AFogAnimationInteraction : public AAnimationInteraction
{
	GENERATED_BODY()
	
public:
	AFogAnimationInteraction();


	UPROPERTY()
		TArray<UBoxComponent*> BoxCompArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComp2;


	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginTriggerEvent()override;
	virtual void EndTriggerEvent()override;
	virtual void EnableEvent()override;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
