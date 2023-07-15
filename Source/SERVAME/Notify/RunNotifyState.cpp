// Fill out your copyright notice in the Description page of Project Settings.


#include "RunNotifyState.h"

void URunNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Boss = Cast<AJesusBoss>(MeshComp->GetOwner());
		if (Boss)
		{
			Boss->IsRunArrived = false;
			Boss->SetLockOn();
			Boss->OnStart();
		}
	}
}

void URunNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Boss = Cast<AJesusBoss>(MeshComp->GetOwner());
		if (Boss && !Boss->IsRunArrived)
		{
			Boss->IsRunArrived = true;
			Boss->SetLockOff();
			Boss->OnEnd();
		}
	}
}

void URunNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Boss = Cast<AJesusBoss>(MeshComp->GetOwner());
		if (Boss)
		{
			Dist = FVector::Dist(Boss->GetActorLocation(), Boss->PlayerCharacter->GetActorLocation());

			if (Dist <= ArriveDistance && !Boss->IsRunArrived)
			{
				Boss->IsRunArrived = true;
				Boss->SetLockOff();
				Boss->OnEnd();
			}
		}
	}
}
