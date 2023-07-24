// Fill out your copyright notice in the Description page of Project Settings.


#include "RunNotifyState.h"

void URunNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

		if (BaseCharacter)
		{
			//todo
			visit_at(GetBoss(MeshComp), BossEnumType.GetIntValue(), [=](auto& val)
				{
					UE_LOG(LogTemp, Warning, TEXT("Call_Templet_lamda"));

					val->IsRunArrived = false;
					val->SetLockOn();
					val->OnStart();
				});
		}
	}
}

void URunNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

		if (BaseCharacter)
		{			
			//todo
			visit_at(GetBoss(MeshComp), BossEnumType.GetIntValue(), [=](auto& val)
				{
					if (!val->IsRunArrived)
					{
						val->IsRunArrived = true;
						val->SetLockOff();
						val->OnEnd();
					}
				});
		}
	}
}

void URunNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

		if (BaseCharacter)
		{
			//todo
			visit_at(GetBoss(MeshComp), BossEnumType.GetIntValue(), [=](auto& val)
				{
					Dist = FVector::Dist(val->GetActorLocation(), val->PlayerCharacter->GetActorLocation());

					if (Dist <= ArriveDistance && !val->IsRunArrived)
					{
						val->IsRunArrived = true;
						val->SetLockOff();
						val->OnEnd();
					}
				});
		}
	}
}
