// Fill out your copyright notice in the Description page of Project Settings.


#include "InterpNotifyState.h"

UInterpNotifyState::UInterpNotifyState()
{
	InterpFucn.Add(InterpType::LERP, TFunction<FVector(ABaseCharacter*)>([=](ABaseCharacter* Temp)
		{						
			return FVector();
		}));

	InterpFucn.Add(InterpType::LERP, TFunction<FVector(ABaseCharacter*)>([=](ABaseCharacter* Temp)
		{
			return FVector();
		}));
}

void UInterpNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
		
		if (BaseCharacter)
		{
			visit_at(GetBoss(MeshComp), BossEnumType.GetIntValue(), [=](auto& val)
				{
					val->IsArrived = false;
					val->LastPlayerLoc = val->PlayerCharacter->GetActorLocation();
				});
		}
	}
}

void UInterpNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (BaseCharacter)
		{
			visit_at(GetBoss(MeshComp), BossEnumType.GetIntValue(), [=](auto& val)
				{
					val->IsArrived = false;
				});
		}
	}
}

void UInterpNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (BaseCharacter)
		{
			visit_at(GetBoss(MeshComp), BossEnumType.GetIntValue(), [=](auto& val)
				{
					if (!val->IsArrived)
					{
						auto NewLoc = val->Lerp(val->GetActorLocation(), val->PlayerCharacter->GetActorLocation(), InterpVal);
						val->SetActorLocation(NewLoc);
					}

					auto Dist = FVector::Dist(val->GetActorLocation(), val->PlayerCharacter->GetActorLocation());

					if (Dist < 100.f)
					{
						val->IsArrived = true;
						return;
					}
				});
		}
	}
}
