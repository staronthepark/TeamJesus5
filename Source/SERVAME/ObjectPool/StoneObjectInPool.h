// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectInPool.h"
#include "Components/BoxComponent.h"
#include "..\Boss_2\JesusBoss2.h"
#include "NiagaraComponent.h"
#include "StoneObjectInPool.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API AStoneObjectInPool : public ABaseObjectInPool
{
	GENERATED_BODY()

	AStoneObjectInPool();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stone")
	float LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stone")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stone")
	float Damage;

	AJesusBoss2* Boss2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stone")
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere)
	UBoxComponent* ProjectileCollision;

	UPROPERTY()
	UJesusGameInstance* GameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stone")
	UStaticMeshComponent* StoneMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stone")
	TObjectPtr<UNiagaraComponent> BurstEffect;

	FTimerHandle LifeTimer;
	FDamageEvent DamageEvent;
	FVector MoveDir;

	virtual void SetActive(bool active) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void ReturnObject()override;

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
