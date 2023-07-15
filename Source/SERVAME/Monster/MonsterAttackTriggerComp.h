// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\BoxTriggerComp.h"
#include "EnemyMonster.h"
#include "MonsterAttackTriggerComp.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SERVAME_API UMonsterAttackTriggerComp : public UBoxTriggerComp
{
	GENERATED_BODY()
public:
	UMonsterAttackTriggerComp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		MonsterAnimationType AttackAnimType;
private:
	AEnemyMonster* Owner;

public:

	virtual void BeginPlay() override;

	virtual void BeginEvent()override;
	virtual void EndEvent()override;
};