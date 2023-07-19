// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCheckNotifyState.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"

void UAttackCheckNotifyState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Boss = Cast<AJesusBoss>(MeshComp->GetOwner());
		
		//int num = -1;
		//
		//auto temp = visit_at(Test(MeshComp), BossEnumType.GetIntValue());

		//auto Boss = std::get<0>(Test(MeshComp));
		


		if (Boss)
		{
			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, false, Boss);

			auto Type = Boss->GetTypeFromMetaData(Boss->GetCurrentMontage());

			bool bResult = Boss->GetWorld()->SweepSingleByChannel(
				OUT HitResult,
				Boss->GetActorLocation(),
				Boss->GetActorLocation(),
				FQuat::Identity,
				ECollisionChannel::ECC_GameTraceChannel3,
				FCollisionShape::MakeSphere(AttackRadius),
				Params);

			FVector Center = Boss->GetActorLocation();
			FColor DrawColor;

			if (bResult)
				DrawColor = FColor::Green;
			else
				DrawColor = FColor::Red;

			//DrawDebugSphere(Boss->GetWorld(), Center, AttackRadius, 16, DrawColor, false, 1.f);

			Boss->CameraShake(Boss->PlayerCameraShake);
			Boss->VibrateGamePad(1.0f, 0.5f);

			if (bResult && HitResult.GetActor())
			{
				UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
				FDamageEvent DamageEvent;
				auto Player = Cast<APlayerCharacter>(HitResult.GetActor());

				Boss->Damage += Boss->BossDataStruct.DamageList[Type];

				Player->TakeDamage(Boss->Damage, DamageEvent, Boss->GetController(), Boss);
				//			}
			}
		}
	}
}

std::tuple<AJesusBoss*, AJesusBoss2*> UAttackCheckNotifyState::Test(USkeletalMeshComponent* MeshComp)
{
	return std::tuple<AJesusBoss*, AJesusBoss2*>(Cast<AJesusBoss>(MeshComp->GetOwner()), Cast<AJesusBoss2>(MeshComp->GetOwner()));
}

