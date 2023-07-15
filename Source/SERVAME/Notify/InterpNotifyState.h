// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "..\Boss_1\JesusBoss.h"
#include "InterpNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UInterpNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	AJesusBoss* Boss;

public:
	UPROPERTY(EditAnywhere)
	float InterpVal;
};
