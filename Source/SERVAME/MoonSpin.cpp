// Fill out your copyright notice in the Description page of Project Settings.


#include "MoonSpin.h"

// Sets default values
AMoonSpin::AMoonSpin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	StaticMeshComp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Comp"));
}

// Called when the game starts or when spawned
void AMoonSpin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoonSpin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Rotation += DeltaTime * RotateSpeed;
	SetActorRotation(FRotator(Rotation, Rotation, 0));
}

