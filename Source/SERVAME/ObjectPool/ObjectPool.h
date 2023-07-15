// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseObjectInPool.h"
#include "Engine/World.h"
#include "ObjectPool.generated.h"


USTRUCT(BlueprintType)
struct FObjectInPoolInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectInPoolInfo")
	TSubclassOf<class ABaseObjectInPool> ObjClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectInPoolInfo")
	int CreateNum;
};

UCLASS(Meta = (DisableNativeTick))
class SERVAME_API AObjectPool : public AActor
{
	GENERATED_BODY()

	AObjectPool();
public:	

	static AObjectPool* Instance;
	static AObjectPool& GetInstance();
	
	UPROPERTY()
	UWorld* World;

	UPROPERTY(EditAnyWhere, Category = "ObejctArray")
	TArray<FObjectInPoolInfo> ObjectArray;

		TMap<TSubclassOf<class ABaseObjectInPool>, TArray<ABaseObjectInPool*>> ObjectPool;

	void CreateNewObjectInPool(TSubclassOf<class ABaseObjectInPool> ObjectInPool);

	void AddNewObjectTypeInPool(TSubclassOf<class ABaseObjectInPool> ObjectInPool, int CreateObjectCount);

	ABaseObjectInPool* SpawnObject(TSubclassOf<class ABaseObjectInPool> ObjectInPool, FVector location, FRotator rotation);

	void ReturnObject(TSubclassOf<class ABaseObjectInPool> ObjectInPool, ABaseObjectInPool* object);

	virtual void BeginPlay() override;
};