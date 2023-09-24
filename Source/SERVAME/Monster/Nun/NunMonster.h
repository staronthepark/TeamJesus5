// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\EnemyMonster.h"
#include "..\..\Player\PlayerCharacter.h"
#include "..\..\UI\MonsterHPUI.h"
#include "..\..\UI\MonsterWidget.h"
#include "Components/WidgetComponent.h"
#include "NumAnimInstance.h"
#include "..\Knight\KinghtMonster.h"
#include "NunMonster.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API ANunMonster : public AEnemyMonster
{
	GENERATED_BODY()
	
public:
	ANunMonster();

	UPROPERTY()
	UNumAnimInstance* NunAnimInstance;

	TArray<UBoxComponent*> SpawnLocArr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* ProjectileRootComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Loc1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Loc2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Loc3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Loc4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Loc5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Loc6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* CrystalSpawnLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNunAttackTriggerComp* AttackTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackRadius")
	float FirstRange = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackRadius")
	float SecondRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knight")
	TSubclassOf<AKinghtMonster> KnightClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knight")
	TArray<AKinghtMonster*> KnightArr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knight")
	int KnightNum = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knight")
	float KnightSpawnRadius = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knight")
	float KnightSpawnVal = 0.2f;

	TArray<AActor*> TeleportArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeleportVal")
	float TeleportVal = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeleportVal")
	float TeleportDelayVal = 0.1f;
	int CurrentNum = 0;
	FTimerHandle TeleportTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float HealVal = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float MultiHealVal = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float HealRadius = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float SelfHealCoolTime = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float SelfHealVal = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DotAttack")
	float DotRange = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DotAttack")
	float DotTime = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JudementAttack")
	float JudementRange = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JudementAttack")
	float JudementTime = 15.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JudementAttack")
	float JudementDelay = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JudementAttack")
	int JudementMaxCount = 15;
	int JudementCurrentCount = 0;
	FTimerHandle JudementTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CrystalAttack")
	float CrystalRange = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CrystalAttack")
	int CrystalCount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PrayAttack")
	float PrayObjSpawnDelay = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PrayDamage")
	float PrayDamage = 5.f;
	int PraySpawnCount = 0;

	float TeleportDamageSum = 0.f;
	float SpawnDamageSum = 0.f;
	float IllusionDamageSum = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FragmentsAttack")
	float FragmentRange = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "fasdfasfasdfasdfasd")
	bool test = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illusion")
	TSubclassOf<ANunMonster> IllusionNunClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illusion")
	float IllusionTime = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illusion")
	float IllusionVal = 0.25f;
	bool useIllusion = false;
	bool IsIllusion = false;
	bool MinusOpacity = false;
	FTimerHandle IllusionTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaternDelay")
	int MinDelayTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaternDelay")
	int MaxDelayTime = 3.f;
	bool IsCoolTime = false;

	//수녀 a타입 스킬
	void TelePort();
	void SpawnKnight();
	void SingleHeal();
	void MultiHeal();
	void SelfHeal();
	void DotFloor();
	void JudementAttack();
	void Curse();

	//수녀 b타입 스킬
	void CrystalAttack();
	void FogAttack();
	void PrayAttack();
	void FragmentsAttack();
	void IllusionAttack();

	FTimerHandle SelfHealTimerHandle;
	FTimerHandle DelayTimerHandle;
	FTimerHandle PaternDelay;

	void SetYaw();

	UFUNCTION()
	void OnNunTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnNunTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void StartAttackTrigger(MonsterAnimationType AttackAnimType) override;
	virtual void EndAttackTrigger(MonsterAnimationType AttackAnimType) override;

	virtual float Die(float Dm) override;

	virtual void Stun() override;

	virtual void MonsterHitStop() override;

	virtual void ChangeMontageAnimation(MonsterAnimationType type) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void CheckMontageEndNotify() override;

	virtual void PlayExecutionAnimation() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void IsNotifyActive(bool value) override;
	virtual void RespawnCharacter() override;

	virtual void ResumeMontage() override;

};
