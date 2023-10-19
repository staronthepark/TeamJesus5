// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShakeNotify.h"
#include "../SERVAME/Player/PlayerCharacter.h"

void UCameraShakeNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	APlayerCharacter* character = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (character != nullptr)
	{
		character->CameraShake(character->PlayerCameraShake);
	}
}