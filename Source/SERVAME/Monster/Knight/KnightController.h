// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KnightController.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API AKnightController : public AAIController
{
	GENERATED_BODY()
	
public:
	AKnightController();

	virtual void BeginPlay();

	void Movement(FVector Location);
};
