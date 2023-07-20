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
DECLARE_MULTICAST_DELEGATE(FOnRightEnable);
DECLARE_MULTICAST_DELEGATE(FOnRightDisable);
DECLARE_MULTICAST_DELEGATE(FOnLeftEnable);
DECLARE_MULTICAST_DELEGATE(FOnLeftDisable);
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
	void AnimNotify_OnRightEnable();
	UFUNCTION()
	void AnimNotify_OnRightDisable();
	UFUNCTION()
	void AnimNotify_OnLeftEnable();
	UFUNCTION()
	void AnimNotify_OnLeftDisable();
	UFUNCTION()
	void AnimNotify_LockOn();
	UFUNCTION()
	void AnimNotify_LockOff();

public:
	FOnCrossFall OnCrossFall;
	FOnStart OnStart;
	FOnEnd OnEnd;
	FOnRightEnable OnRightEnable;
	FOnRightDisable OnRightDisable;
	FOnLeftEnable OnLeftEnable;
	FOnLeftDisable OnLeftDisable;
	FOnLockOn OnLockOn;
	FOnLockOff OnLockOff;
};
