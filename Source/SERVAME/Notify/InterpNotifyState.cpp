// Fill out your copyright notice in the Description page of Project Settings.


#include "InterpNotifyState.h"

void UInterpNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Boss = Cast<AJesusBoss>(MeshComp->GetOwner());
		
		if (Boss)
		{
			Boss->IsArrived = false;
			Boss->LastPlayerLoc = Boss->PlayerCharacter->GetActorLocation();
		}
	}
}

void UInterpNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (Boss)
		{
			Boss->IsArrived = false;
		}
	}
}

void UInterpNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (Boss)
		{
			if (!Boss->IsArrived)
			{
				auto NewLoc = Boss->Lerp(Boss->GetActorLocation(), Boss->PlayerCharacter->GetActorLocation(), InterpVal);
				Boss->SetActorLocation(NewLoc);
			}

			auto Dist = FVector::Dist(Boss->GetActorLocation(), Boss->PlayerCharacter->GetActorLocation());
			
			if (Dist < 100.f)
			{
				Boss->IsArrived = true;
				return;
			}
		}
	}
}
