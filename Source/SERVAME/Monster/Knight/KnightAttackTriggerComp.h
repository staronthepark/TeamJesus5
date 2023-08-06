// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\..\BoxTriggerComp.h"
#include "KinghtMonster.h"
#include "KnightAttackTriggerComp.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SERVAME_API UKnightAttackTriggerComp : public UBoxTriggerComp
{
	GENERATED_BODY()
public:
	UKnightAttackTriggerComp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	KnightAnimationType AttackAnimType;

private:
	AKinghtMonster* Owner;

public:

	virtual void BeginPlay() override;

	virtual void BeginEvent()override;
	virtual void EndEvent()override;
};