// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "..\Player\PlayerCharacter.h"
#include "EventNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UEventNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
		APlayerCharacter* Character;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
