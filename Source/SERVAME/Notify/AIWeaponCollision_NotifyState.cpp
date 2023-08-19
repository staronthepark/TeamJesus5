// Fill out your copyright notice in the Description page of Project Settings.


#include "AIWeaponCollision_NotifyState.h"

void UAIWeaponCollision_NotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Boss2 = Cast<AJesusBoss2>(MeshComp->GetOwner());

		if (Boss2)
		{
			Boss2->CollisionMap[CollisionType](true);
			Boss2->ActivateTrail();

			if (Left)
				Boss2->ActivateLFOverlap();
			if(Right)
				Boss2->ActivateRFOverlap();
		}
	}
}

void UAIWeaponCollision_NotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Boss2 = Cast<AJesusBoss2>(MeshComp->GetOwner());

		if (Boss2)
		{
			Boss2->CollisionMap[CollisionType](false);
			Boss2->DeactivateTrail();
			Boss2->DeactivateLFOverlap();
			Boss2->DeactivateRFOverlap();
			Boss2->Damage = 0.f;
		}
	}
}
