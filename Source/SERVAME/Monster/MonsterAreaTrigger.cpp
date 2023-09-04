// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAreaTrigger.h"

UMonsterAreaTrigger::UMonsterAreaTrigger()
{
}

void UMonsterAreaTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void UMonsterAreaTrigger::BeginEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerIn"));
}

void UMonsterAreaTrigger::EndEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Out"));
}
