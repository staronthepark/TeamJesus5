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
	STINGaTTACK1,
	HEAL1,
	HEAL2,
	SELFHEAL,
	WORSHIP,
	JUDEMENT,
	CURSE,
	DARK,
	PRAY,
	SPAWNKNIGHT,
	SPAWNING,
	REVIVE,
	FRAGMENT,
	CRYSTAL,
	ILLUSION,
	SPRINTATTACK,

	FORWARDMOVE,
	LEFTMOVE,
	RIGHTMOVE,
	BACKMOVE,

	ROTATELEFT,
	ROTATERIGHT,

	HIT,
	BACKHIT,
	DEAD,
	DEADLOOP,
	EXECUTION,
	STARTDEAD,
	PARRYING,
	GROGGY_START,
	GROGGY_LOOP,
	SUPER_HIT,
	GROGGY_DEAD,

	ELITEKNIGHT_STAND_IDLE,
	ELITEKNIGHT_START,
	JAMSIG_SIT_IDLE,
	JAMSIG_STANDUP,
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
