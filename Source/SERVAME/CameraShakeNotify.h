// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CameraShakeNotify.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UCameraShakeNotify : public UAnimNotifyState
{
	GENERATED_BODY()
		virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);

};
