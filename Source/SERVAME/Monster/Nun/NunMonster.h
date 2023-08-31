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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNunAttackTriggerComp* AttackTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knight")
	TArray<AKinghtMonster*> KnightArr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knight")
	int KnightNum = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> TeleportArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeleportVal")
	float TeleportVal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float HealVal;

	void TelePort();
	void SpawnKnight();
	void SingleHeal();
	void MultiHeal();

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
