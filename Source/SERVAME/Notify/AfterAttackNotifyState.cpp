// Fill out your copyright notice in the Description page of Project Settings.


#include "AfterAttackNotifyState.h"

void UAfterAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->AfterAttackNotify(true);
		}
	}
}

void UAfterAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->AfterAttackNotify(false);
		}
	}
}