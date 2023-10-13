// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\..\BoxTriggerComp.h"
#include "Jamsig.h"
#include "JamsigAttackTriggerComp.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SERVAME_API UJamsigAttackTriggerComp : public UBoxTriggerComp
{
	GENERATED_BODY()
public:
	UJamsigAttackTriggerComp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	MonsterAnimationType AttackAnimType;

private:
	AJamsig* Owner;

public:

	virtual void BeginPlay() override;
	virtual void BeginEvent()override;
	virtual void EndEvent()override;
};
