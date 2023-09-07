// Fill out your copyright notice in the Description page of Project Settings.


#include "NunDamageSphereTriggerComp.h"

UNunDamageSphereTriggerComp::UNunDamageSphereTriggerComp()
{
}

void UNunDamageSphereTriggerComp::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UNunDamageSphereTriggerComp::BeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UNunDamageSphereTriggerComp::EndOverlap);

	Nun = Cast<ANunMonster>(UGameplayStatics::GetActorOfClass(GetWorld(), ANunMonster::StaticClass()));
	IsActiveHit = false;

	Count = 0;
}

void UNunDamageSphereTriggerComp::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Character == nullptr)
		Character = Cast<APlayerCharacter>(OtherActor);

	if (!IsActiveHit)
	{
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &UNunDamageSphereTriggerComp::ActiveHit, DamageTime);
		IsActiveHit = true;
	}
}

void UNunDamageSphereTriggerComp::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IsActiveHit = false;
	GetWorld()->GetTimerManager().ClearTimer(Timer);
}

void UNunDamageSphereTriggerComp::ActiveHit()
{
	Count++;

	if (Character->TakeDamage(Damage, DamageEvent, nullptr, GetOwner()))
	{
		AObjectPool& objectpool = AObjectPool::GetInstance();
		objectpool.SpawnObject(objectpool.ObjectArray[8].ObjClass, Character->GetActorLocation(), FRotator(90, 180, 180));
		objectpool.SpawnObject(objectpool.ObjectArray[9].ObjClass, Character->GetActorLocation(), FRotator(90, 180, 180));
		objectpool.SpawnObject(objectpool.ObjectArray[3].ObjClass, Character->GetActorLocation(), FRotator(90, 180, 180));
	}

	if (Count >= MaxCount)
	{
		Count = 0;
		IsActiveHit = false;

		return;
	}
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UNunDamageSphereTriggerComp::ActiveHit, DamageTime);
}
