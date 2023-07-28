// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\BaseAnimInstance.h"
#include "BossAnimInstance.generated.h"

UENUM(BlueprintType)
enum class BossAnimationType : uint8
{
	NONE,
	LEVELSTART,
	RUN,
	RUN_L,
	RUN_R,
	ATTACK,
	IDLE,
	RANGEATTACK,
	STUN,
	GROGGY,
	UPPERSLASH,
	CHARGEATTACK,
	BackSwing,
	BACKSTEP,
	RIGHTSTEP,
	LEFTSTEP,
	JUMPATTACK,
	DARKEXPLOSION,
	DOWNATTACK,
	FASTSLASH,
	THRIDSLASH,
	THIRDJUMP,
	GROUNDEXPLOSION,
	EXECUTION,
	GROGGYIDLE,
	DIE,
	HIT,
	POWERHIT,
	SPRINT,
	CRY,
	STANDUP,
};

DECLARE_MULTICAST_DELEGATE(FOnLevelStart);
DECLARE_MULTICAST_DELEGATE(FOnActionStart);
DECLARE_MULTICAST_DELEGATE(FOnActionEnd);
DECLARE_MULTICAST_DELEGATE(FOnRangeAttack);
DECLARE_MULTICAST_DELEGATE(FOnEnable);
DECLARE_MULTICAST_DELEGATE(FOnDisable);
DECLARE_MULTICAST_DELEGATE(FOnAttackCheck);
DECLARE_MULTICAST_DELEGATE(FOnImmediatelyLockOn);
DECLARE_MULTICAST_DELEGATE(FOnLockOn);
DECLARE_MULTICAST_DELEGATE(FOnLockOff);
DECLARE_MULTICAST_DELEGATE(FOnChangeDamage);
DECLARE_MULTICAST_DELEGATE(FOnOverlapEnable);
DECLARE_MULTICAST_DELEGATE(FOnOverlapDisable);
DECLARE_MULTICAST_DELEGATE(FOnTrailActivate);
DECLARE_MULTICAST_DELEGATE(FOnTrailDeActivate);
DECLARE_MULTICAST_DELEGATE(FOnExplosion);
DECLARE_MULTICAST_DELEGATE(FOnJump);
DECLARE_MULTICAST_DELEGATE(FOnJumpMove);
DECLARE_MULTICAST_DELEGATE(FOnJumpMoveMiddle);
DECLARE_MULTICAST_DELEGATE(FOnJumpMoveEnd);
DECLARE_MULTICAST_DELEGATE(FOnSetParticleLoc);
DECLARE_MULTICAST_DELEGATE(FOnReturnStun);
DECLARE_MULTICAST_DELEGATE(FOnExecution);


/**
 * 
 */
UCLASS()
class SERVAME_API UBossAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

public:
	UBossAnimInstance();

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Horizontal;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Vertical;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsStart = false;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsSequenceEnd = false;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FRotator CurrentBoneRotateVal = FRotator::ZeroRotator;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FRotator BoneRotateVal = FRotator::ZeroRotator;

	void PlayGroggyMontage(BossAnimationType Type);

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
	void AnimNotify_End();

	UFUNCTION()
	void AnimNotify_RangeTemp();

	UFUNCTION()
	void AnimNotify_Enable();

	UFUNCTION()
	void AnimNotify_Disable();

	UFUNCTION()
	void AnimNotify_OverlapEnable();

	UFUNCTION()
	void AnimNotify_OverlapDisable();

	UFUNCTION()
	void AnimNotify_ChangeDamage();

	UFUNCTION()
	void AnimNotify_AttackCheck();

	UFUNCTION()
	void AnimNotify_ImmediatelyLockOn();

	UFUNCTION()
	void AnimNotify_LockOn();

	UFUNCTION()
	void AnimNotify_LockOff();

	UFUNCTION()
	void AnimNotify_TrailOn();

	UFUNCTION()
	void AnimNotify_TrailOff();

	UFUNCTION()
	void AnimNotify_Explosion();

	UFUNCTION()
	void AnimNotify_JumpMove();

	UFUNCTION()
	void AnimNotify_JumpMoveMiddle();

	UFUNCTION()
	void AnimNotify_JumpMoveEnd();

	UFUNCTION()
	void AnimNotify_OnStart();

	UFUNCTION()
	void AnimNotify_OnEnd();

	UFUNCTION()
	void AnimNotify_OnSetParticleLoc();

	UFUNCTION()
	void AnimNotify_OnReturnStun();

	UFUNCTION()
	void AnimNotify_OnExecution();

public:
	FOnLevelStart OnLevelStart;
	FOnRangeAttack OnRangeAttack;
	FOnEnable OnEnable;
	FOnDisable OnDisable;
	FOnAttackCheck OnAttackCheck;
	FOnImmediatelyLockOn OnImmediatelyLockOn;
	FOnLockOn OnLockOn;
	FOnLockOff OnLockOff;
	FOnChangeDamage OnChangeDamage;
	FOnOverlapDisable OnOverlapDisable;
	FOnOverlapEnable OnOverlapEnable;
	FOnTrailActivate OnTrailActivate;
	FOnTrailDeActivate OnTrailDeActivate;
	FOnExplosion OnExplosion;
	FOnJump OnJump;
	FOnJumpMove OnJumpMove;
	FOnJumpMoveMiddle OnJumpMiddle;
	FOnJumpMoveEnd OnJumpMoveEnd;
	FOnActionStart OnStart;
	FOnActionEnd OnEnd;
	FOnSetParticleLoc OnSetParticleLoc;
	FOnReturnStun OnReturnStun;
	FOnExecution OnExecution;
};
