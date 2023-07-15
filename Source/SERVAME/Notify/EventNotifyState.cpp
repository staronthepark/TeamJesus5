// Fill out your copyright notice in the Description page of Project Settings.


#include "EventNotifyState.h"

void UEventNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Character = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Character)
		Character->EventNotify(true);
}

void UEventNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Character = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Character)
		Character->EventNotify(false);
}
