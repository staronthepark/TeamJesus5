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

