// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\EnemyMonster.h"
#include "..\Jamsig\JamsigAniminstance.h"
#include "..\..\Player\PlayerCharacter.h"
#include "..\..\UI\MonsterHPUI.h"
#include "..\..\UI\MonsterWidget.h"
#include "Components/WidgetComponent.h"
#include "Jamsig.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API AJamsig : public AEnemyMonster
{
	GENERATED_BODY()

public:
	AJamsig();
	
	UPROPERTY()
	UJamsigAniminstance* JamsigAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UJamsigAttackTriggerComp* AttackTrigger;

	bool MinusOpacity = false;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	APlayerCharacter* Player;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsKnockBack = false;
	UPROPERTY(EditAnyWhere, Category = "KnockBackTime")
	float KnockBackTime = 0.2f;
	UPROPERTY(EditAnyWhere, Category = "KnockBackDelayTime")
	float KnockBackDelayTime = 1.f;
	UPROPERTY(EditAnywhere, Category = "Jamsig")
	bool SitJamsig = false;

	FTimerHandle KnockBackTimerHandle;
	FTimerHandle KnockBackDelayTimerHandle;

	MonsterAnimationType HitType;

	void ActivateAttackTrigger();
	void DeactivateAttackTrigger();

	//Notify
	void KnockBackStart();
	void KnockBackEnd();

	UFUNCTION()
	void OnJamsigTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnJamsigTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void StartAttackTrigger(MonsterAnimationType AttackAnimType) override;
	virtual void EndAttackTrigger(MonsterAnimationType AttackAnimType) override;

	virtual float Die(float Dm) override;

	virtual void Stun() override;
	virtual void ParryingStun() override;

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
