// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAttackTriggerComp.h"

UMonsterAttackTriggerComp::UMonsterAttackTriggerComp()
{
}

void UMonsterAttackTriggerComp::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AEnemyMonster>(GetOwner());
}

void UMonsterAttackTriggerComp::BeginEvent()
{
	Owner->StartAttackTrigger(AttackAnimType);
}

void UMonsterAttackTriggerComp::EndEvent()
{
	Owner->EndAttackTrigger(AttackAnimType);
}