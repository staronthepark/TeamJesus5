// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectObjectInPool.h"

AEffectObjectInPool::AEffectObjectInPool()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CurrentEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CurrentEffect"));
	CurrentEffect->SetupAttachment(RootComponent);
}


void AEffectObjectInPool::BeginPlay()
{
	Super::BeginPlay();
}

void AEffectObjectInPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEffectObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);

	if (LifeTime > 0 && active)
		GetWorldTimerManager().SetTimer(LifeTimer, this, &AEffectObjectInPool::ReturnObject, LifeTime);
}

void AEffectObjectInPool::ReturnObject()
{
	Super::ReturnObject();
	SetActorTickEnabled(false);
}

void AEffectObjectInPool::SetEffectType(SelectEffectType type)
{
	Type = type;
	CurrentEffect->SetAsset(GetTypeEffect[Type]);
}

void AEffectObjectInPool::ActivateCurrentEffect()
{
	CurrentEffect->Activate();
}

void AEffectObjectInPool::DeactivateCurrentEffect()
{
	CurrentEffect->DeactivateImmediate();
}
