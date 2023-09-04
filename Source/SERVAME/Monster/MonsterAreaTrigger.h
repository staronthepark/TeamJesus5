// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyMonster.h"
#include "..\BoxTriggerComp.h"
#include "MonsterAreaTrigger.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UMonsterAreaTrigger : public UBoxTriggerComp
{
	GENERATED_BODY()
	
public:
	UMonsterAreaTrigger();

	TArray<AEnemyMonster*> AreaMonsterArr;

	virtual void BeginPlay() override;

	virtual void BeginEvent()override;
	virtual void EndEvent()override;
};
