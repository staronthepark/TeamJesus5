// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\..\BaseAnimInstance.h"
#include "KnightAnimInstance.generated.h"

UENUM(BlueprintType)
enum class KnightAnimationType : uint8
{
	NONE,
	IDLE,
	ATTACK1,
	POWERATTACK1,
	DASHATTACK1,

	FORWARDMOVE,
	LEFTMOVE,
	RIGHTMOVE,
	BACKMOVE,

	HIT,
	DEAD,
	DEADLOOP,
	EXECUTION,
};


UCLASS()
class SERVAME_API UKnightAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
public:
	UKnightAnimInstance() {};
};
