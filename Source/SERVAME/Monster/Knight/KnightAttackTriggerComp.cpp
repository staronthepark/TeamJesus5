// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightAttackTriggerComp.h"

UKnightAttackTriggerComp::UKnightAttackTriggerComp()
{
}

void UKnightAttackTriggerComp::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AKinghtMonster>(GetOwner());
}

void UKnightAttackTriggerComp::BeginEvent()
{
	Owner->StartAttackTrigger(AttackAnimType);
}

void UKnightAttackTriggerComp::EndEvent()
{
	Owner->EndAttackTrigger(AttackAnimType);
}