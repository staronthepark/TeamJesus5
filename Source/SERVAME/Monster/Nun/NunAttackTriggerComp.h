// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NunMonster.h"
#include "..\..\BoxTriggerComp.h"
#include "NunAttackTriggerComp.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UNunAttackTriggerComp : public UBoxTriggerComp
{
	GENERATED_BODY()
	
public:
	UNunAttackTriggerComp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		MonsterAnimationType AttackAnimType;

private:
	ANunMonster* Owner;

public:
	virtual void BeginPlay() override;

	virtual void BeginEvent()override;
	virtual void EndEvent()override;
};
