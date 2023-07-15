// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCheckNotifyState.h"
#include "DrawDebugHelpers.h"

void UAttackCheckNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		Boss = Cast<AJesusBoss>(MeshComp->GetOwner());

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

//			if (bResult && HitResult.Actor.IsValid())
//			{
//				UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName());
//				FDamageEvent DamageEvent;
//				auto Player = Cast<APlayerCharacter>(HitResult.Actor);
//
//				Boss->Damage += Boss->BossDataStruct.DamageList[Type];
//
//				Player->TakeDamage(Boss->Damage, DamageEvent, Boss->GetController(), Boss);
//			}
		}
	}
}
