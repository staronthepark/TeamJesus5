// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalObjectInPool.h"

ADecalObjectInPool::ADecalObjectInPool()
{
	DecalComp = CreateDefaultSubobject<UDecalComponent>("Decal");
	PrimaryActorTick.bCanEverTick = true;
}

void ADecalObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);
	DecalComp->SetActive(active, false);
	if (active)
	{	//(Pitch=-90.000015,Yaw=-184.413147,Roll=4.413300)
		SetActorRotation(FRotator(-90, FMath::RandRange(0, 360), 4));
		Opacity = 1.0f;
	}
	if (LifeTime > 0.0)
		GetWorldTimerManager().SetTimer(LifeTimeHandler, this, &ADecalObjectInPool::ReturnObject, LifeTime);
	SetActorTickEnabled(true);
	Location = GetActorLocation();
}

void ADecalObjectInPool::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

}

void ADecalObjectInPool::ReturnObject()
{
	Super::ReturnObject();
	SetActorTickEnabled(false);
}

void ADecalObjectInPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Location = GetActorLocation();
	SetActorLocation(FVector(Location.X, Location.Y, Location.Z -= DeltaTime * 90.0f));
}