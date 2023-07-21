// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCheckNotifyState.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"

UAttackCheckNotifyState::UAttackCheckNotifyState()
{
}

void UAttackCheckNotifyState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
		
		if (!BaseCharacter)
			return;

		//auto Boss = std::get<0>(Test(MeshComp));
		//visit_at(Test(MeshComp), BossEnumType.GetIntValue(), this->fboss1);

		visit_at(GetBoss(MeshComp), BossEnumType.GetIntValue(), [=](auto& val)
			{
				UE_LOG(LogTemp, Warning, TEXT("Call_Templet_lamda"));
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, false, val);

				auto Type = val->GetTypeFromMetaData(val->GetCurrentMontage());

				bool bResult = val->GetWorld()->SweepSingleByChannel(
					OUT HitResult,
					val->AreaAtkPos->GetComponentLocation(),
					val->AreaAtkPos->GetComponentLocation(),
					FQuat::Identity,
					ECollisionChannel::ECC_GameTraceChannel3,
					FCollisionShape::MakeSphere(AttackRadius),
					Params);

				FVector Center = val->AreaAtkPos->GetComponentLocation();
				FColor DrawColor;

				if (bResult)
					DrawColor = FColor::Green;
				else
					DrawColor = FColor::Red;

				DrawDebugSphere(val->GetWorld(), Center, AttackRadius, 16, DrawColor, false, 1.f);

				val->CameraShake(val->PlayerCameraShake);
				val->VibrateGamePad(1.0f, 0.5f);

				if (bResult && HitResult.GetActor())
				{
					auto Player = Cast<APlayerCharacter>(HitResult.GetActor());

					if (Player)
					{
						UE_LOG(LogTemp, Warning, TEXT("Current Atk : %s"), *val->GetCurrentMontage()->GetName());
						UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
						FDamageEvent DamageEvent;

						val->Damage += val->BossDataStruct.DamageList[Type];

						Player->TakeDamage(val->Damage, DamageEvent, val->GetController(), val);
					}
				}
			});
	}
}
