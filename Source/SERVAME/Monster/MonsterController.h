// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterController.generated.h"


UCLASS()
class SERVAME_API AMonsterController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterController();

	virtual void BeginPlay();

	bool IsArrived = false;

	void Movement(FVector Location);
	void MoveWhenArrived(FVector Location);

	class AEnemyMonster* Monster;
private:
};