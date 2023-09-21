// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ObjectPool/BaseObjectInPool.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"
#include "PlayerSkillObject.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API APlayerSkillObject : public ABaseObjectInPool
{
	GENERATED_BODY()

	APlayerSkillObject();

	FDamageEvent DamageEvent;
	FTimerHandle LifeTimer;

public:

	UPROPERTY(EditAnywhere)
		USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere)
	UBoxComponent* HitCollider;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* NiagraComp;

	UPROPERTY(EditAnywhere)
		float Damage;

	UPROPERTY(EditAnywhere)
		float LifeTime;

	virtual void BeginPlay() override;

	virtual void SetActive(bool active) override;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};