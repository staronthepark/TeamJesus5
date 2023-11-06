// Fill out your copyright notice in the Description page of Project Settings.


#include "../BaseCharacter.h"
#include "../Player/PlayerCharacter.h"
#include "../ObjectPool/PlayerSkillObject.h"
#include "../Manager/CombatManager.h"

APlayerSkillObject::APlayerSkillObject()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>("Scene Comp");
	SceneComp->SetupAttachment(RootComponent);
	HitCollider = CreateDefaultSubobject<UBoxComponent>("Hit Collider");
	HitCollider->SetupAttachment(SceneComp);
	NiagraComp = CreateDefaultSubobject<UNiagaraComponent>("Niagra Comp");
	NiagraComp->SetupAttachment(SceneComp);
}

void APlayerSkillObject::BeginPlay()
{
	Super::BeginPlay();

	HitCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerSkillObject::OverlapBegin);
}

void APlayerSkillObject::SetActive(bool active)
{
	Super::SetActive(active);

	if (active)
	{
		GetWorldTimerManager().SetTimer(LifeTimer, this, &APlayerSkillObject::ReturnObject, LifeTime);
		NiagraComp->SetActive(true, false);

		APlayerCharacter* character = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		Damage = character->PlayerDataStruct.BaseDamage * character->PlayerDataStruct.DamageList[AnimationType::SKILL1].Damage;

		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);

		bool bResult = GetWorld()->SweepSingleByChannel(
			OUT HitResult,
			GetActorLocation(),
			GetActorLocation(),
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel18,
			FCollisionShape::MakeSphere(200.0f),
			Params);

		//FColor DrawColor;
		//
		//if (bResult)
		//	DrawColor = FColor::Green;
		//else
		//	DrawColor = FColor::Red;
		//
		//DrawDebugSphere(GetWorld(), GetActorLocation(), 200.0f, 16, DrawColor, false, 2.f);

		if (bResult && HitResult.GetActor())
		{
			FDamageEvent DamageEvent;
			auto Player = Cast<ABaseCharacter>(HitResult.GetActor());

			if (Player != nullptr)
			{
				Player->TakeDamage(Damage, DamageEvent, nullptr, this);
			}
		}
	}
}

void APlayerSkillObject::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}