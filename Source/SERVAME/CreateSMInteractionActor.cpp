// Fill out your copyright notice in the Description page of Project Settings.

#include "CreateSMInteractionActor.h"

ACreateSMInteractionActor::ACreateSMInteractionActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACreateSMInteractionActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> array = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	
	for (int32 i = 0; i < array.Num(); i++)
	{
		MeshCompArray.Add(Cast<UStaticMeshComponent>(array[i]));
		MeshCompArray[i]->SetVisibility(false);
	}
	
	array = GetComponentsByClass(UParticleSystemComponent::StaticClass());
	for (int32 i = 0; i < array.Num(); i++)
	{
		ParticleCompArray.Add(Cast<UParticleSystemComponent>(array[i]));
		ParticleCompArray[i]->SetVisibility(false);
	}
}

void ACreateSMInteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACreateSMInteractionActor::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();
}

void ACreateSMInteractionActor::EndTriggerEvent()
{
	Super::EndTriggerEvent();
}

void ACreateSMInteractionActor::EnableEvent()
{
	Super::EnableEvent();
	for (int32 i = 0; i < MeshCompArray.Num(); i++)
	{
		MeshCompArray[i]->SetVisibility(true);
	}

	for (int32 i = 0; i < ParticleCompArray.Num(); i++)
	{
		ParticleCompArray[i]->SetVisibility(true);
	}
}