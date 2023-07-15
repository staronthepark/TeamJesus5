// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "..\BaseCharacter.h"
#include "AnimNotifyEndState.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UAnimNotifyEndState : public UAnimNotifyState
{
	GENERATED_BODY()

	ABaseCharacter* Character;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
};