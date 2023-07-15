// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyEndState.h"


void UAnimNotifyEndState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->CheckMontageEndNotify();
	}
}