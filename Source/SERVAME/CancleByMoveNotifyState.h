// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CancleByMoveNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UCancleByMoveNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

		virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
		virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
