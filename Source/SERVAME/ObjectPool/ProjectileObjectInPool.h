// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectInPool.h"
#include "Components/BoxComponent.h"
#include "..\BaseCharacter.h"
#include "ProjectileObjectInPool.generated.h"
/**
 * 
 */
UCLASS()
class SERVAME_API AProjectileObjectInPool : public ABaseObjectInPool
{
	GENERATED_BODY()

	AProjectileObjectInPool();
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
	float LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
	float Speed;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle)
	//UParticleSystemComponent* ParticleSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle)
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Collision1;

	//UPROPERTY()
//	FDamageEvent DamageEvent;

	UPROPERTY()
	UJesusGameInstance* GameInstance;

	FTimerHandle LifeTimer;

	UPROPERTY()
	FVector MoveDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
	bool ReturnWhenHit;

	virtual void SetActive(bool active) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void ReturnObject()override;

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};