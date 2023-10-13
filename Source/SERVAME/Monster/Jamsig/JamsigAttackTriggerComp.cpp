// Fill out your copyright notice in the Description page of Project Settings.


#include "JamsigAttackTriggerComp.h"

UJamsigAttackTriggerComp::UJamsigAttackTriggerComp()
{
}

void UJamsigAttackTriggerComp::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AJamsig>(GetOwner());
}

void UJamsigAttackTriggerComp::BeginEvent()
{
	Owner->StartAttackTrigger(AttackAnimType);
}

void UJamsigAttackTriggerComp::EndEvent()
{
	Owner->EndAttackTrigger(AttackAnimType);
}
