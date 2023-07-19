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
};

DECLARE_MULTICAST_DELEGATE(FOnCrossFall);
DECLARE_MULTICAST_DELEGATE(FOnStart);
DECLARE_MULTICAST_DELEGATE(FOnEnd);
DECLARE_MULTICAST_DELEGATE(FOnEnable);
DECLARE_MULTICAST_DELEGATE(FOnDisable);
DECLARE_MULTICAST_DELEGATE(FOnLockOn);
DECLARE_MULTICAST_DELEGATE(FOnLockOff);

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

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
	void AnimNotify_CrossFall();
	UFUNCTION()
	void AnimNotify_OnStart();
	UFUNCTION()
	void AnimNotify_OnEnd();
	UFUNCTION()
	void AnimNotify_OnEnable();
	UFUNCTION()
	void AnimNotify_OnDisable();
	UFUNCTION()
	void AnimNotify_LockOn();
	UFUNCTION()
	void AnimNotify_LockOff();

public:
	FOnCrossFall OnCrossFall;
	FOnStart OnStart;
	FOnEnd OnEnd;
	FOnEnable OnEnable;
	FOnDisable OnDisable;
	FOnLockOn OnLockOn;
	FOnLockOff OnLockOff;
};
