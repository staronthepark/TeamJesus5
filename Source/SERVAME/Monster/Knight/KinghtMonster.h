// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\EnemyMonster.h"
#include "..\..\Player\PlayerCharacter.h"
#include "..\..\UI\MonsterHPUI.h"
#include "..\..\UI\MonsterWidget.h"
#include "Components/WidgetComponent.h"
#include "KnightAnimInstance.h"
#include "KnightArmor.h"
#include "KinghtMonster.generated.h"

UCLASS()
class SERVAME_API AKinghtMonster : public AEnemyMonster
{
	GENERATED_BODY()
public:
	AKinghtMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	TSubclassOf<AKnightArmor> ArmorClass;

	AKnightArmor* KnightArmor;

	UPROPERTY()
	UKnightAnimInstance* KnightAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UKnightAttackTriggerComp* AttackTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UKnightAttackTriggerComp* DashAttackTrigger;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnyWhere,Category = "KnockBackTime")
	float KnockBackTime = 0.2f;
	UPROPERTY(EditAnyWhere, Category = "KnockBackDelayTime")
	float KnockBackDelayTime = 1.f;

	bool IsInterpStart;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsKnockBack = false;

	TArray<FVector> CirclePoints;
	UPROPERTY(EditAnyWhere, Category = "Circle Walk")
	int NumSegments = 16;
	UPROPERTY(EditAnyWhere, Category = "Circle Walk")
	float Radius = 700.f;
	UPROPERTY(EditAnyWhere, Category = "Circle Walk")
	float CircleWalkMinTime = 3.f;
	UPROPERTY(EditAnyWhere, Category = "Circle Walk")
	bool DrawDebugCircle = false;
	bool CircleWalkEnd = false;

private:

	FTimerHandle KnockBackTimerHandle;
	FTimerHandle KnockBackDelayTimerHandle;
	FTimerHandle CircleWalkTimerHandle;

	//Notify
	void InterpStart();
	void InterpEnd();
	void KnockBackStart();
	void KnockBackEmd();

public:
	void InterpMove();
	void ActivateAttackTrigger();
	void DeactivateAttackTrigger();
	void Rotate();
	void DrawCircle(FVector Center);


	UFUNCTION()
	void OnKnightTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnKnightTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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