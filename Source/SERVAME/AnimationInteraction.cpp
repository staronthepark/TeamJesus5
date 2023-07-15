// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationInteraction.h"

AAnimationInteraction::AAnimationInteraction()
{
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMesh"));

	RootComponent = MeshComp;

}

void AAnimationInteraction::BeginPlay()
{
	Super::BeginPlay();
}

void AAnimationInteraction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAnimationInteraction::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();
}

void AAnimationInteraction::EndTriggerEvent()
{
	Super::EndTriggerEvent();

}

void AAnimationInteraction::EnableEvent()
{
	Super::EnableEvent();

	//play anim
}