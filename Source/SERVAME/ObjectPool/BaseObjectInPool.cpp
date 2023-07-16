// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseObjectInPool.h"
#include "ObjectPool.h"

ABaseObjectInPool::ABaseObjectInPool()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActive(false);
}

void ABaseObjectInPool::BeginPlay()
{
	Super::BeginPlay();
	SetActive(false);
}

void ABaseObjectInPool::SetActive(bool active)
{
	SetActorHiddenInGame(!active);
	SetActorEnableCollision(active);
	SetActorTickEnabled(active);
}

void ABaseObjectInPool::ReturnObject()
{	
	AObjectPool::GetInstance().ReturnObject(SubClass, this);
}