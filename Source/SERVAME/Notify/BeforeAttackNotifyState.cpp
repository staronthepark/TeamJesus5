// Fill out your copyright notice in the Description page of Project Settings.


#include "BeforeAttackNotifyState.h"

void UBeforeAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->BeforeAttackNotify(true);
		}
	}
}

void UBeforeAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->BeforeAttackNotify(false);
		}
	}
}