// Fill out your copyright notice in the Description page of Project Settings.


#include "CancleByMoveNotifyState.h"
#include "../SERVAME/Player/PlayerCharacter.h"

void UCancleByMoveNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	APlayerCharacter* character = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (character != nullptr)
	{
		character->AfterAttackNotify2(true);
	}
}

void UCancleByMoveNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APlayerCharacter* character = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (character != nullptr)
	{
		character->AfterAttackNotify2(false);
	}
}
