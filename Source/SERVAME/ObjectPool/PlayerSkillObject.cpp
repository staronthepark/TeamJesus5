// Fill out your copyright notice in the Description page of Project Settings.


#include "../BaseCharacter.h"
#include "../Player/PlayerCharacter.h"
#include "../ObjectPool/PlayerSkillObject.h"

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
	}
}

void APlayerSkillObject::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);

	if (character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit"));
		character->TakeDamage(Damage, DamageEvent, nullptr, this);
	}
}