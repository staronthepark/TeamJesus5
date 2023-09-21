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
#include "KnightPatrolSplineActor.h"
#include "KinghtMonster.generated.h"

UCLASS()
class SERVAME_API AKinghtMonster : public AEnemyMonster
{
	GENERATED_BODY()
public:
	AKinghtMonster();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float DistanceAlongSpline = 0.f;

	UPROPERTY()
	UKnightAnimInstance* KnightAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AKnightPatrolSplineActor*> PatrolActorArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* KnightHeadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UKnightAttackTriggerComp* AttackTrigger;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnyWhere,Category = "KnockBackTime")
	float KnockBackTime = 0.2f;
	UPROPERTY(EditAnyWhere, Category = "KnockBackDelayTime")
	float KnockBackDelayTime = 1.f;

	bool IsInterpStart;

	bool Spawning = false;

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

	UPROPERTY(EditAnyWhere, Category = "RunableDistance")
	float RunableDistance = 1000.f;
	UPROPERTY(EditAnyWhere, Category = "RunableDistance")
	float AccelerationDist = 700.f;
	float InterpolationDuration = 0.5f;
	float InterpolationTime = 0.0f;
	bool WalkToRunBlend;

	bool StartRun;
	float CalcedDist;
	float Temp = 0.f;

	bool CanCancle = true;

	const float IdleBlend = 0.f;
	const float WalkBlend = 300.f;
	const float RunBlend = 600.f;

	bool MinusOpacity = false;
	bool IsSpawn = false;

	bool IsMoveStart;
	float fDeltaTime;
	float MinWalkTime;

	//Notify
	void InterpStart();
	void InterpEnd();
	void KnockBackStart();
	void KnockBackEmd();
	void SpawnBegin();
	void SpawnEnd();
	void OnHitCancle();
	void OffHitCancle();

private:

	FTimerHandle KnockBackTimerHandle;
	FTimerHandle KnockBackDelayTimerHandle;
	FTimerHandle CircleWalkTimerHandle;

	MonsterAnimationType HitType;

public:
	void InterpMove();
	void ActivateAttackTrigger();
	void DeactivateAttackTrigger();
	void Rotate();
	void DrawCircle(FVector Center);
	void SearchPlayer();
	float GetRandNum(float Min, float Max);

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