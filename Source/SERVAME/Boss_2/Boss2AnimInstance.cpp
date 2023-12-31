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

void UBoss2AnimInstance::AnimNotify_VomitFall() { OnVomitFall.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_OnStart() { OnStart.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_OnEnd() { OnEnd.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_LockOn() { OnLockOn.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_LockOff() { OnLockOff.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_JumpStart() { OnJumpStart.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_JumpEnd() { OnJumpEnd.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_ThrowStone() { OnStoneThrow.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_LeftLockOn() { OnLeftLockOn.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_LeftLockOff() { OnLeftLockOff.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_RightLockOn() { OnRightLockOn.Broadcast(); }

void UBoss2AnimInstance::AnimNotify_RightLockOff() { OnRightLockOff.Broadcast(); }
