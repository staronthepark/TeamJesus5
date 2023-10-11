// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightAnimInstance.h"

UKnightAnimInstance::UKnightAnimInstance()
{

}
void UKnightAnimInstance::AnimNotify_InterpStart() { InterpStart.Broadcast(); }

void UKnightAnimInstance::AnimNotify_InterpEnd() { InterpEnd.Broadcast(); }

void UKnightAnimInstance::AnimNotify_KnockBackStart() { KnockBackStart.Broadcast(); }

void UKnightAnimInstance::AnimNotify_KnockBackEnd() { KnockBackEnd.Broadcast(); }

void UKnightAnimInstance::AnimNotify_SpawningBegin() { SpawningBegin.Broadcast(); }

void UKnightAnimInstance::AnimNotify_SpawningEnd() { SpawningEnd.Broadcast(); }

void UKnightAnimInstance::AnimNotify_OnHitCancle() { CanHitCancle.Broadcast(); }

void UKnightAnimInstance::AnimNotify_OffHitCancle() { CantHitCancle.Broadcast(); }

void UKnightAnimInstance::AnimNotify_OnRotate() { CanRotate.Broadcast(); }

void UKnightAnimInstance::AnimNotify_OffRotate() { CantRotate.Broadcast(); }
