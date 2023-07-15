// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "..\Boss_1\JesusBoss.h"
#include "RunNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API URunNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	AJesusBoss* Boss;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);

	float Dist = FLT_MAX;
public:
	UPROPERTY(EditAnywhere)
	float ArriveDistance;
};
