// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleObjectInPool.h"

AParticleObjectInPool::AParticleObjectInPool()
{
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleSystem->SetupAttachment(RootComponent);
	ParticleSystem->SetCollisionProfileName("Particle");

	MoveComp = CreateDefaultSubobject<UMoveToLocationComp>(TEXT("Movement"));


	PrimaryActorTick.bCanEverTick = false;
}

void AParticleObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);
	ParticleSystem->SetActive(active, false);
	if (active && LifeTime > 0)
	{
		GetWorldTimerManager().SetTimer(LifeTimer, this, &AParticleObjectInPool::ReturnObject, LifeTime);
		if (IsMove)
		{
			MoveComp->SetComponentTickEnabled(true);
			//MoveComp->SetTargetLocation();
		}
	}
}