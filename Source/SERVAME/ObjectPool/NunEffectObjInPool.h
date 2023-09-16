// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectInPool.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "..\BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NunEffectObjInPool.generated.h"

UENUM(BlueprintType)
enum class EffectType : uint8
{
	NONE,
	MAGICCIRCLE,
	SINGLEHEAL,
	MULTIHEAL,
	SELFHEAL,
	HEALDUST,
	WORSHIPEFFECT,
	JUDEMENTEFFECT,
	DARKEFFECT,
	DARKEFFECTHIT,
	FOGEFFECT,
	PRAYEFFECT,
	PRAYHITEFFECT,
	FRAGMENTEFFECT_CHARGE,
	FRAGMENTEFFECT_BURST,
	JUDGEMENTEFFECT,
	SALVATIONEFFECT,
	CRYSTALEFFECT,
	CRYSTALEFFECT_BUSRT,
};

UCLASS()
class SERVAME_API ANunEffectObjInPool : public ABaseObjectInPool
{
	GENERATED_BODY()
	
public:
	ANunEffectObjInPool();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNunDamageSphereTriggerComp* DamageSphereTriggerComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectType")
	TMap<EffectType, UNiagaraSystem*> GetTypeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectType")
	EffectType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NunEffect")
	float LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NunEffect")
	float Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, Category = "CurrentEffect")
	TObjectPtr<UNiagaraComponent> CurrentEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<USphereComponent> ProjectileCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Speed;
	FVector MoveDir;
	bool IsShot = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<USphereComponent> RangeAttackCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Delay = 1.f;

	UPROPERTY()
	FDamageEvent DamageEvent;

	FTimerHandle LifeTimer;
	FTimerHandle DotTimerHandle;
	FTimerHandle ShotTimerHandle;

	TMap<EffectType, EffectType> GetBurstEffectType;

	virtual void SetActive(bool active) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void ReturnObject()override;

	void ShotProjectile(ABaseCharacter* Player);
	void ShotProjectile(FVector Target);

	void SetCurrentEffect(EffectType type);

	void ActivateCurrentEffect();
	void DeactivateCurrentEffect();

	void DeactivateDamageSphere(float time);

	UFUNCTION()
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnRangeAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
