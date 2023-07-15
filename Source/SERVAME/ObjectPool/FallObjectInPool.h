// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectInPool.h"
#include "Components/BoxComponent.h"
#include "..\BaseCharacter.h"
#include "FallObjectInPool.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API AFallObjectInPool : public ABaseObjectInPool
{
	GENERATED_BODY()

	AFallObjectInPool();
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
	float LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
	float FallSpeed;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle)
	//UParticleSystemComponent* ParticleSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
	UBoxComponent* HitCollision;

	UPROPERTY(EditAnywhere)
	UBoxComponent* GroundHitCollision;

//	UPROPERTY()
//	FDamageEvent DamageEvent;

	UPROPERTY()
	UJesusGameInstance* GameInstance;

	FTimerHandle LifeTimer;

	FVector MoveDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
	float Damage;

	bool IsHitGround = false;

	virtual void SetActive(bool active) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void ReturnObject()override;

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnGroundOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
