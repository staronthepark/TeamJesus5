#include "ObjectPool.h"
#include "Engine/World.h"
#include "../Manager/CombatManager.h"
#include "UObject/ConstructorHelpers.h"

AObjectPool* AObjectPool::Instance = nullptr;

AObjectPool::AObjectPool()
{
	World = nullptr;
	
	Instance = this;

	PrimaryActorTick.bCanEverTick = false;
}

void AObjectPool::BeginPlay()
{
	World = GetWorld();
	for (int32 i = 0; i < ObjectArray.Num(); i++)
	{
		AddNewObjectTypeInPool(ObjectArray[i].ObjClass, ObjectArray[i].CreateNum);
	}
}

AObjectPool& AObjectPool::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<AObjectPool>();
	}
	return *Instance;
}

void AObjectPool::CreateNewObjectInPool(TSubclassOf<class ABaseObjectInPool> ObjectInPool)
{
	if (World)
	{
		ABaseObjectInPool* object = World->SpawnActor<ABaseObjectInPool>(ObjectInPool);
		ObjectPool[ObjectInPool].Add(object);
		object->SetActorLocation(FVector(10000, 10000, 10000));
	}
}

void AObjectPool::AddNewObjectTypeInPool(TSubclassOf<class ABaseObjectInPool> ObjectInPool, int CreateObjectCount)
{
	ObjectPool.Add(ObjectInPool, TArray<ABaseObjectInPool*>());
	ABaseObjectInPool* object;
	for (int32 i = 0; i < CreateObjectCount; i++)
	{
		object = World->SpawnActor<ABaseObjectInPool>(ObjectInPool);
		object->SetActorLocation(FVector(10000, 10000, 10000));
		ObjectPool[ObjectInPool].Add(object);
	}
}

ABaseObjectInPool* AObjectPool::SpawnObject(TSubclassOf<class ABaseObjectInPool> ObjectInPool, FVector location, FRotator rotation)
{
	if (ObjectPool[ObjectInPool].Num() > 0)
	{
		ABaseObjectInPool* object = ObjectPool[ObjectInPool][0];
		object->SetActorLocation(location);
		object->SetActorRotation(rotation);
		object->SetActive(true);
		ObjectPool[ObjectInPool].RemoveAt(0);
		return object;
	}
	else
	{
		CreateNewObjectInPool(ObjectInPool);
		SpawnObject(ObjectInPool, location, rotation);
	}

	return nullptr;
}

void AObjectPool::ReturnObject(TSubclassOf<class ABaseObjectInPool> ObjectInPool, ABaseObjectInPool* object)
{
	object->SetActorLocation(FVector(10000, 10000, 10000));
	object->SetActive(false);
	ObjectPool[ObjectInPool].Add(object);
}