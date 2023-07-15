// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectInPool.h"
#include "Components/DecalComponent.h"
#include "DecalObjectInPool.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API ADecalObjectInPool : public ABaseObjectInPool
{
	GENERATED_BODY()

public:
		ADecalObjectInPool();

		UPROPERTY(Editanywhere, BlueprintReadWrite)
		float LifeTime;

		UPROPERTY(Editanywhere, BlueprintReadWrite)
			UDecalComponent* DecalComp;



		float Opacity;
private:
		FTimerHandle LifeTimeHandler;

		FVector Location;


public:
		virtual void SetActive(bool active) override;
		virtual void BeginPlay()override;
		virtual void ReturnObject()override;
		virtual void Tick(float DeltaTime) override;
};
