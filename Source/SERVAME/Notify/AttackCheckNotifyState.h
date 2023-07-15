// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "..\Boss_1\JesusBoss.h"
#include "..\Player\PlayerCharacter.h"
#include "AttackCheckNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UAttackCheckNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	AJesusBoss* Boss;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
public:
	UPROPERTY(EditAnywhere)
	float AttackRadius;
};
