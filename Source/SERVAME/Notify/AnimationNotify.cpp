// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationNotify.h"

void UAnimationNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->IsNotifyActive(true);
		}
	}
}

void UAnimationNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->IsNotifyActive(false);
		}
	}
}