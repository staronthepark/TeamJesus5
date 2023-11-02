// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectInPool.h"
#include "Components/BoxComponent.h"
#include "..\BaseCharacter.h"
#include "..\Boss_2\JesusBoss2.h"
#include "Engine/DamageEvents.h"
#include "NiagaraComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* CrossEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* CrossBurstEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* DustEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
	UBoxComponent* HitCollision;

	UPROPERTY(EditAnywhere)
	UBoxComponent* GroundHitCollision;

	UPROPERTY()
	FDamageEvent DamageEvent;

	UPROPERTY()
	UJesusGameInstance* GameInstance;

	AJesusBoss2* Boss2;

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
