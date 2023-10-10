// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "BossBaseNotifyState.h"
#include "InterpNotifyState.generated.h"

/**
 * 
 */

UCLASS()
class SERVAME_API UInterpNotifyState : public UBossBaseNotifyState
{
	GENERATED_BODY()
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);

public:
	UInterpNotifyState();

	UPROPERTY(EditAnywhere)
	float InterpVal;
};


