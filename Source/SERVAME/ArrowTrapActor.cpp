// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowTrapActor.h"
#include "./ObjectPool/ObjectPool.h"

AArrowTrapActor::AArrowTrapActor()
{
}

void AArrowTrapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArrowTrapActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsTimer)
	{
		GetWorldTimerManager().SetTimer(Timer, this, &AArrowTrapActor::EnableTrap, Time);
	}
}

void AArrowTrapActor::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();
}

void AArrowTrapActor::EndTriggerEvent()
{
	Super::EndTriggerEvent();
}

void AArrowTrapActor::EnableEvent()
{
	Super::EnableEvent();

	if (!IsTimer)
		EnableTrap();
}

void AArrowTrapActor::EnableTrap()
{
	AObjectPool& ObjectPool = AObjectPool::GetInstance();

	ObjectPool.SpawnObject(ObjectPool.ObjectArray[45].ObjClass, GetActorLocation(), FRotator::ZeroRotator);


	if (IsTimer)
		GetWorldTimerManager().SetTimer(Timer, this, &AArrowTrapActor::EnableTrap, Time);

}