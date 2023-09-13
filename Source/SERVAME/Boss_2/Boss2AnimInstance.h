// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\BaseAnimInstance.h"
#include "Boss2AnimInstance.generated.h"


UENUM(BlueprintType)
enum class Boss2AnimationType : uint8
{
	NONE,
	IDLE,
	CROSSFALL,
	FOWARDWALK,
	LEFTWALK,
	DOWNSMASH,
	SLASH,
	DOUBLESMASH,
	DIE,
	SCREAMATTACK,
	HEADATTACK,
	CHARGE,
	HEADING,
	VOMITFALL,
	ELBOWSPIN,
	HUNTJUMP,
	JUMPEXPLOSION,
	THROWSTONE,
	FOLLOWUP_CHARGE,
	FOLLOWUP_SCREAMATTACK,
	LEFT_TURN_ATTACK,
	RIGHT_TURN_ATTACK,
	GROGGY,
};

UENUM(BlueprintType)
enum class Boss2BoneRotateType : uint8
{
	HEAD = 0,
	LEFTARM,
	RIGHTARM,
	TURNHEAD,
};

DECLARE_MULTICAST_DELEGATE(FOnCrossFall);
DECLARE_MULTICAST_DELEGATE(FOnVomitFall);
DECLARE_MULTICAST_DELEGATE(FOnStart);
DECLARE_MULTICAST_DELEGATE(FOnEnd);
DECLARE_MULTICAST_DELEGATE(FOnLockOn);
DECLARE_MULTICAST_DELEGATE(FOnLockOff);
DECLARE_MULTICAST_DELEGATE(FOnJumpMoveStart);
DECLARE_MULTICAST_DELEGATE(FOnJumpMoveEnd);
DECLARE_MULTICAST_DELEGATE(FOnStoneThorw);
DECLARE_MULTICAST_DELEGATE(FOnLeftLockOn);
DECLARE_MULTICAST_DELEGATE(FOnLeftLockOff);
DECLARE_MULTICAST_DELEGATE(FOnRightLockOn);
DECLARE_MULTICAST_DELEGATE(FOnRightLockOff);

UCLASS()
class SERVAME_API UBoss2AnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
public:
	UBoss2AnimInstance();

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsStart = false;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FRotator CurrentBoneRotateVal = FRotator::ZeroRotator;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FRotator HeadBoneRotateVal = FRotator::ZeroRotator;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FRotator LArmBoneRotateVal = FRotator::ZeroRotator;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FRotator RArmBoneRotateVal = FRotator::ZeroRotator;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FRotator LClavicle = FRotator::ZeroRotator;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FRotator RClavicle = FRotator::ZeroRotator;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FVector LookAtPos;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	Boss2BoneRotateType CurrentBoneType;

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
	void AnimNotify_CrossFall();
	UFUNCTION()
	void AnimNotify_VomitFall();
	UFUNCTION()
	void AnimNotify_OnStart();
	UFUNCTION()
	void AnimNotify_OnEnd();
	UFUNCTION()
	void AnimNotify_LockOn();
	UFUNCTION()
	void AnimNotify_LockOff();
	UFUNCTION()
	void AnimNotify_JumpStart();
	UFUNCTION()
	void AnimNotify_JumpEnd();
	UFUNCTION()
	void AnimNotify_ThrowStone();
	UFUNCTION()
	void AnimNotify_LeftLockOn();
	UFUNCTION()
	void AnimNotify_LeftLockOff();
	UFUNCTION()
	void AnimNotify_RightLockOn();
	UFUNCTION()
	void AnimNotify_RightLockOff();
public:
	FOnCrossFall OnCrossFall;
	FOnVomitFall OnVomitFall;
	FOnStart OnStart;
	FOnEnd OnEnd;
	FOnLockOn OnLockOn;
	FOnLockOff OnLockOff;
	FOnJumpMoveStart OnJumpStart;
	FOnJumpMoveEnd OnJumpEnd;
	FOnStoneThorw OnStoneThrow;
	FOnLeftLockOn OnLeftLockOn;
	FOnLeftLockOff OnLeftLockOff;
	FOnRightLockOn OnRightLockOn;
	FOnRightLockOff OnRightLockOff;
};
