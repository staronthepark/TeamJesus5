// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "UObject/Class.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include ".\ObjectPool\ObjectPool.h"
#include ".\Manager\JesusGameInstance.h"
#include "Engine/World.h"
#include "BaseTriggerActor.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "BaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	LOW,
	MIDDLE,
	HIGH,
};

UENUM()
enum BossType : uint32
{
	BOSS_1 = 0,
	BOSS_2,
};

USTRUCT(BlueprintType)
struct FCharacterBaseDataStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CharacterMaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CharacterHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CharacterOriginSpeed;
};

UCLASS()
class SERVAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();	

protected:	

public:
	AActor* TargetActor;

	FTimerHandle TimerHandler;
	FTimerHandle SMOverlapTimerHandler;
	FDamageEvent CharacterDamageEvent;

	FVector SpawnLocation;
	FRotator SpawnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EArmorType ArmorType;

	bool Imotal;
	bool IsLockOn;
	bool IsPlayer;
	bool IsStun = false;
	bool CanExecution = false;

	float RotSpeed;
	float fDeltaTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf< UCameraShakeBase> PlayerCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* WeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* HitCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* ParryingCollision1;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* WeaponOverlapStaticMeshCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* TargetDetectionCollison;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* SwordTrailComp;

	UPROPERTY()
	UJesusGameInstance* GameInstance;
	USphereComponent* LockOnComp;

private:
	UPROPERTY()
	TArray<UBoxComponent*> DustColliderArray;
	UPROPERTY()
	TArray<UCapsuleComponent*> HitColliderArray;

public:
	virtual void BeginPlay();
	virtual void PostInitializeComponents();	
	virtual void PlayExecutionAnimation() {}
	virtual void ActivateLockOnImage(bool value, UPrimitiveComponent* comp) {}
	virtual void SetActive(bool active);

	virtual void Tick(float DeltaTime);

	virtual void BeforeAttackNotify(bool value) {}
	virtual void AfterAttackNotify(bool value) {}
	virtual void IsNotifyActive(bool value) { }
	virtual void CheckMontageEndNotify() {}

	virtual void ActivateRightWeapon()  { WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  }
	virtual void DeactivateRightWeapon(){ WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); }
	virtual void ActivateSMOverlap()  { WeaponOverlapStaticMeshCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  }
	virtual void DeactivateSMOverlap() {WeaponOverlapStaticMeshCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);   }

	virtual void ActivateHitCollision();
	void DeactivateHitCollision();

	virtual void RespawnCharacter();

	virtual void ResumeMontage(){ HitStopTimer(); }

	virtual void HitStop(){ HitStopTimer(); }
	virtual void Stun(){}
	void HitStopTimer();

	void SwordVFXSpawn();

	virtual bool IsAlive() { return 0; }

	void CameraShake(TSubclassOf< UCameraShakeBase> Shake);

	void VibrateGamePad(float Intensity, float time);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION()
	void OnDustCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};