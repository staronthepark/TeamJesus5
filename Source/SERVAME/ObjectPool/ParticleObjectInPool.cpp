// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleObjectInPool.h"

AParticleObjectInPool::AParticleObjectInPool()
{
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleSystem->SetupAttachment(RootComponent);
	ParticleSystem->SetCollisionProfileName("Particle");

	PrimaryActorTick.bCanEverTick = false;
}

void AParticleObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);
	ParticleSystem->SetActive(active, false);
	if (active)
	{
		GetWorldTimerManager().SetTimer(LifeTimer, this, &AParticleObjectInPool::ReturnObject, LifeTime);
	}
}