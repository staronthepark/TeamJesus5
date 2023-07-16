// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseObjectInPool.generated.h"

UENUM(BlueprintType)
enum class ObjectType : uint8
{
	NONE,
	MONSTER,
	PROJECTILE,
	ITEM,
	VFX,
	SFX,
};

UCLASS()
class SERVAME_API ABaseObjectInPool : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseObjectInPool();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABaseObjectInPool> SubClass;


public:	

	virtual void BeginPlay();

	virtual void SetActive(bool active);
	virtual void ReturnObject();

};
