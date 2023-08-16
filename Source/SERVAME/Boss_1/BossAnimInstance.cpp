// Fill out your copyright notice in the Description page of Project Settings.

#include "JesusBoss.h"
#include "BossAnimInstance.h"

UBossAnimInstance::UBossAnimInstance()
{
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();

	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();
	}
}

void UBossAnimInstance::PlayGroggyMontage(BossAnimationType Type)
{
	auto Boss = Cast<AJesusBoss>(GetOwningActor());
	
	AObjectPool& objectpool = AObjectPool::GetInstance();
	for (int32 i = 0; i < Boss->BossDataStruct.DropSoulCount; i++)
	{
		float x = FMath::RandRange(-300.0f, 300.0f);
		float y = FMath::RandRange(-300.0f, 300.0f);
		float z = FMath::RandRange(-300.0f, 300.0f);

		FVector location = Boss->GetActorLocation() + FVector(x * 0.1f, y * 0.1f, z * 0.1f);
		FRotator rotation = Boss->GetActorRotation() + FRotator(x, y, z);

		objectpool.SpawnObject(objectpool.ObjectArray[36].ObjClass, location, rotation);
	}

	if (Boss->BossDataStruct.CurrentGrrogyGauge <= 0)
	{
		Boss->ChangeMontageAnimation(Type);
		Boss->WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Boss->IsActionEnd = false;
		Boss->IsStun = true;
		Boss->BossDataStruct.CurrentGrrogyGauge = Boss->BossDataStruct.MaxGrrogyGauge;
		return;
	}

	if (Boss->IsParriged && !Boss->IsStun)
	{
		Boss->ChangeMontageAnimation(Type);
		Boss->IsActionEnd = false;
		Boss->IsStun = true;
		return;
	}
	else
	{
		Boss->ChangeMontageAnimation(Type);
		Boss->IsActionEnd = false;
		Boss->IsStun = true;
		return;
	}
}

void UBossAnimInstance::AnimNotify_End() { OnLevelStart.Broadcast(); }

void UBossAnimInstance::AnimNotify_RangeTemp() { OnRangeAttack.Broadcast(); }

void UBossAnimInstance::AnimNotify_AttackCheck() { OnAttackCheck.Broadcast(); }

void UBossAnimInstance::AnimNotify_Enable() { OnEnable.Broadcast(); }

void UBossAnimInstance::AnimNotify_Disable() { OnDisable.Broadcast(); }

void UBossAnimInstance::AnimNotify_ImmediatelyLockOn() { OnImmediatelyLockOn.Broadcast(); }

void UBossAnimInstance::AnimNotify_LockOn() { OnLockOn.Broadcast(); }

void UBossAnimInstance::AnimNotify_LockOff() { OnLockOff.Broadcast(); }

void UBossAnimInstance::AnimNotify_ChangeDamage() { OnChangeDamage.Broadcast(); }

void UBossAnimInstance::AnimNotify_OverlapEnable() { OnOverlapEnable.Broadcast(); }

void UBossAnimInstance::AnimNotify_OverlapDisable() { OnOverlapDisable.Broadcast(); }

void UBossAnimInstance::AnimNotify_TrailOn() { OnTrailActivate.Broadcast(); }

void UBossAnimInstance::AnimNotify_TrailOff() { OnTrailDeActivate.Broadcast(); }

void UBossAnimInstance::AnimNotify_Explosion() { OnExplosion.Broadcast(); }

void UBossAnimInstance::AnimNotify_JumpMove() { OnJumpMove.Broadcast(); }

void UBossAnimInstance::AnimNotify_JumpMoveMiddle() { OnJumpMiddle.Broadcast(); }

void UBossAnimInstance::AnimNotify_JumpMoveEnd() { OnJumpMoveEnd.Broadcast(); }

void UBossAnimInstance::AnimNotify_OnStart() { OnStart.Broadcast(); }

void UBossAnimInstance::AnimNotify_OnEnd() { OnEnd.Broadcast(); }

void UBossAnimInstance::AnimNotify_OnSetParticleLoc() { OnSetParticleLoc.Broadcast(); }

void UBossAnimInstance::AnimNotify_OnReturnStun() { OnReturnStun.Broadcast(); }

void UBossAnimInstance::AnimNotify_OnExecution() { OnExecution.Broadcast(); }
