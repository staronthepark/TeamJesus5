// Fill out your copyright notice in the Description page of Project Settings.

#include ".\ObjectPool\ObjectPool.h"
#include "SavePointInteraction.h"

ASavePointInteraction::ASavePointInteraction()
{

}


void ASavePointInteraction::BeginPlay()
{
	Super::BeginPlay();

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
		ParticleCompArray[i]->SetActive(IsActive, IsActive);
	}

	Init();
}

void ASavePointInteraction::Init()
{
	for (int32 i = 0; i < ParticleCompArray.Num(); i++)
	{
		ParticleCompArray[i]->SetActive(IsActive, IsActive);
	}
}

void ASavePointInteraction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DissolveValue -= DeltaTime * 0.5f;
	for (int32 i = 0; i < MeshCompArray.Num(); i++)
	{
		MeshCompArray[i]->SetScalarParameterValueOnMaterials("Dissolve", DissolveValue);
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
		Character->CurrentMapName = Character->SaveMapName = Name;
		Character->AxisX = 1;
		Character->AxisY = 1;
		Character->Imotal = true;
		Character->ChangeMontageAnimation(AnimationType::SAVESTART);
		AObjectPool& objectpool = AObjectPool::GetInstance();
		objectpool.SpawnObject(objectpool.ObjectArray[3].ObjClass, GetActorLocation() + FVector(0, 0, 150.0f), FRotator::ZeroRotator);
		objectpool.SpawnObject(objectpool.ObjectArray[23].ObjClass, GetActorLocation() + FVector(0, 0, 200.0f), FRotator::ZeroRotator);
		IsActive = true;

		//Character->GameInstance->PlayerStatUI->AddToViewport();

		for (int32 i = 0; i < ParticleCompArray.Num(); i++)
		{
			ParticleCompArray[i]->SetActive(true, false);
		}
	}
	else
	{
		Character->Imotal = true;
		Character->ChangeMontageAnimation(AnimationType::ACTIVESAVEPOINT);
	}
}