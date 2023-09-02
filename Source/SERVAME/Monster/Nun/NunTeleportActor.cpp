// Fill out your copyright notice in the Description page of Project Settings.


#include "NunTeleportActor.h"

// Sets default values
ANunTeleportActor::ANunTeleportActor()
{
	LocationActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossWeaponMesh"));
	LocationActor->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANunTeleportActor::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ANunTeleportActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

