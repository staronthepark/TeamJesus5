// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossBaseNotifyState.h"
#include "AttackCheckNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UAttackCheckNotifyState : public UBossBaseNotifyState
{
	GENERATED_BODY()

	UAttackCheckNotifyState();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);

public:
	UPROPERTY(EditAnywhere)
	float AttackRadius;
};
