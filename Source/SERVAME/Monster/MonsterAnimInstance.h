// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\BaseAnimInstance.h"
#include "MonsterAnimInstance.generated.h"


UENUM(BlueprintType)
enum class MonsterAnimationType : uint8
{
	NONE,
	STANDBY,
	IDLE,
	ATTACK1,
	POWERATTACK1,
	RANGEATTACK1,
	DASHATTACK1,
	HEAL1,
	HEAL2,
	SPAWNKNIGHT,
	SPAWNING,

	FORWARDMOVE,
	LEFTMOVE,
	RIGHTMOVE,
	BACKMOVE,

	ROTATELEFT,
	ROTATERIGHT,

	HIT,
	DEAD,
	DEADLOOP,
	EXECUTION,
};
/**
 * 
 */
UCLASS()
class SERVAME_API UMonsterAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UMonsterAnimInstance() {};



private:

};
