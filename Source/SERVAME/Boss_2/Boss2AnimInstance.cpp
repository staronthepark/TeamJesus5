// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss2AnimInstance.h"

UBoss2AnimInstance::UBoss2AnimInstance()
{

}

void UBoss2AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();

	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();
	}
}

void UBoss2AnimInstance::AnimNotify_CrossFall() { OnCrossFall.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_OnStart() { OnStart.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_OnEnd() { OnEnd.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_OnRightEnable() { OnRightEnable.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_OnRightDisable() { OnRightDisable.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_OnLeftEnable() { OnLeftEnable.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_OnLeftDisable() { OnLeftDisable.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_LockOn() { OnLockOn.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_LockOff() { OnLockOff.Broadcast(); }
