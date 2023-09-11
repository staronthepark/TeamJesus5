// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\KinghtMonster.h"
#include "DeadBodyOfKinght.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API ADeadBodyOfKinght : public AKinghtMonster
{
	GENERATED_BODY()
	
public:
	ADeadBodyOfKinght();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
