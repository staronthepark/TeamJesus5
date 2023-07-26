// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectInPool.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "..\BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "VomitObjectInPool.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API AVomitObjectInPool : public ABaseObjectInPool
{
	GENERATED_BODY()

	AVomitObjectInPool();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMoveMentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vomit")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vomit")
	float LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vomit")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vomit")
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vomit")
	FVector startLoc = FVector::ZeroVector;      // �߻� ����
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vomit")
	FVector targetLoc = FVector(0, 1000, 1000);  // Ÿ�� ����.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vomit")
	float arcValue = 0.5f;                       // ArcParam (0.0-1.0)

	UPROPERTY(EditAnywhere, Category = "Vomit")
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(EditAnywhere, Category = "Vomit")
	TObjectPtr<USphereComponent> GroundHitCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vomit")
	TObjectPtr<UNiagaraComponent> ProjectileEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vomit")
	TObjectPtr<UNiagaraComponent> BurstEffect;

	UPROPERTY()
	FDamageEvent DamageEvent;

	FTimerHandle LifeTimer;

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
