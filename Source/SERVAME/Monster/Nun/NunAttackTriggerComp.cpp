// Fill out your copyright notice in the Description page of Project Settings.


#include "NunAttackTriggerComp.h"

UNunAttackTriggerComp::UNunAttackTriggerComp()
{
}

void UNunAttackTriggerComp::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ANunMonster>(GetOwner());
}

void UNunAttackTriggerComp::BeginEvent()
{
	Owner->StartAttackTrigger(AttackAnimType);
}

void UNunAttackTriggerComp::EndEvent()
{
	Owner->EndAttackTrigger(AttackAnimType);
}
