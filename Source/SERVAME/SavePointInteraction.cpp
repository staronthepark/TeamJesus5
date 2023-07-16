// Fill out your copyright notice in the Description page of Project Settings.

#include ".\ObjectPool\ObjectPool.h"
#include "SavePointInteraction.h"

ASavePointInteraction::ASavePointInteraction()
{

}

void ASavePointInteraction::BeginPlay()
{
	Super::BeginPlay();

	IsActive = false;


	TArray<UActorComponent *>array = GetComponentsByTag(UStaticMeshComponent::StaticClass(), "Flesh");
	
	DissolveValue = 1.0f;
		 
	
	for (int32 i = 0; i < array.Num(); i++)
	{
		MeshCompArray.Add(Cast<UStaticMeshComponent>(array[i]));
	}
	
	array = GetComponentsByClass(UParticleSystemComponent::StaticClass());
	for (int32 i = 0; i < array.Num(); i++)
	{
		ParticleCompArray.Add(Cast<UParticleSystemComponent>(array[i]));
		ParticleCompArray[i]->SetActive(false, false);
	}
}

void ASavePointInteraction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActive)
	{
		DissolveValue -= DeltaTime * 0.5f;
		for (int32 i = 0; i < MeshCompArray.Num(); i++)
		{
			MeshCompArray[i]->SetScalarParameterValueOnMaterials("Dissolve", DissolveValue);
		}
	}		
}

void ASavePointInteraction::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();
	Character->PlayerHUD->PlayInteractionAnimation(true, EInteractions::cure);
}

void ASavePointInteraction::EndTriggerEvent()
{
	Super::EndTriggerEvent();
	Character->PlayerHUD->PlayInteractionAnimation(false);

}

void ASavePointInteraction::EnableEvent()
{
	Super::EnableEvent();

	GetWorld()->GetFirstPlayerController()->DisableInput(GetWorld()->GetFirstPlayerController());
	if (IsActive)
	{
		Character->Imotal = true;
		Character->ChangeMontageAnimation(AnimationType::SAVESTART);
	}
	else
	{
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[3].ObjClass,  GetActorLocation() + FVector(0, 0, 150.0f), FRotator::ZeroRotator);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[23].ObjClass, GetActorLocation() + FVector(0, 0, 200.0f), FRotator::ZeroRotator);
		IsActive = true;

		for (int32 i = 0; i < ParticleCompArray.Num(); i++)
		{
			ParticleCompArray[i]->SetActive(true, false);
		}
		Character->Imotal = true;
		Character->ChangeMontageAnimation(AnimationType::ACTIVESAVEPOINT);
	}
}