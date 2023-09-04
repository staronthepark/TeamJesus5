// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightPatrolSplineActor.h"

// Sets default values
AKnightPatrolSplineActor::AKnightPatrolSplineActor()
{
	PatrolActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Patrol Spline"));
	PatrolActor->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AKnightPatrolSplineActor::BeginPlay()
{
	Super::BeginPlay();

	if (!IsDebug)
		PatrolActor->SetVisibility(false);
}

// Called every frame
void AKnightPatrolSplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

