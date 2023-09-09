// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\BaseAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UENUM(BlueprintType)
enum class AnimationType : uint8
{
	NONE,

	WALK,
	SPRINT,

	IDLE,
	FORWARDWALK,
	FORWARDBATTLEWALK,
	LEFTWALK,
	RIGHTWALK,
	BACKWALK,
	FORWARDLEFTWALK,
	FORWARDRIGHTWALK,
	BACKLEFTWALK,
	BACKRIGHTWALK,

	BASICDODGE,
	BATTLEDODGE,
	BACKSTEP,

	ATTACK1,
	ATTACK2,
	ATTACK3,
	ATTACK4,
	PARRING,
	POWERATTACK1,
	POWERATTACK2,
	POWERATTACK3,
	DODGEATTACK,

	RUNATTACK,
	RUNPOWERATTACK,
	ENDOFRUN,
	ENDOFSPRINT,
	SPRINTTURN,
	ENDOFHEAL,
	
	HITFRONTLEFT,
	HITFRONTRIGHT,
	HITBACKLEFT,
	HITBACKRIGHT,

	HIT,
	SUPERHIT,


	DOOROPEN,

	DEAD,
	DEADLOOP,
	DEADLOOP2,

	
	HEAL,

	SAVESTART,
	SAVELOOP ,
	SAVEEND  ,
	ACTIVESAVEPOINT,

	GAMESTARTLOOP,
	GAMESTART,

	EXECUTIONBOSS,

	SHIELDSTART,
	SHIELDLOOP,
	SHIELDEND,
	EOSTOEXECUTION,

	SHIELDATTACK,
	SHIELDMOVE,
};

UCLASS()
class SERVAME_API UPlayerAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MyInstance, meta = (AllowPrivateAccess = "true"))
	AnimationType PlayerAnimationType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MyInstance, meta = (AllowPrivateAccess = "true"))
		FRotator HeadBoneRotate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MyInstance, meta = (AllowPrivateAccess = "true"))
		float BodyBlendAlpha;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MyInstance, meta = (AllowPrivateAccess = "true"))
		float PlayerTargetMoveSpeed;
};
