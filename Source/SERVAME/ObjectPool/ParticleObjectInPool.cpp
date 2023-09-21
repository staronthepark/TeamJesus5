// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleObjectInPool.h"
#include "Kismet/GameplayStatics.h"

AParticleObjectInPool::AParticleObjectInPool()
{
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleSystem->SetupAttachment(RootComponent);
	ParticleSystem->SetCollisionProfileName("Particle");

	MoveComp = CreateDefaultSubobject<UMoveToLocationComp>(TEXT("Movement"));

	PrimaryActorTick.bCanEverTick = false;
}

void AParticleObjectInPool::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AObjectPool& objectpool = AObjectPool::GetInstance();
	objectpool.SpawnObject(objectpool.ObjectArray[37].ObjClass, Player->GetActorLocation(), FRotator::ZeroRotator);
	MoveComp->SetComponentTickEnabled(false);
	Player->SetSoul(1);
	ReturnObject();
}

void AParticleObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);
	ParticleSystem->SetActive(active, false);
	if (active)
	{
		if(LifeTime > 0)
		GetWorldTimerManager().SetTimer(LifeTimer, this, &AParticleObjectInPool::ReturnObject, LifeTime);

		MoveComp->SetComponentTickEnabled(IsMove);
		MoveComp->SetTargetLocation(Player);
	}
}

void AParticleObjectInPool::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*>ActorCompArray;
	ActorCompArray = GetComponentsByTag(UBoxComponent::StaticClass(), FName("Overlap"));
	if (ActorCompArray.Num() > 0)
	{
		OverlapComp = Cast<UBoxComponent>(ActorCompArray[0]);
		OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AParticleObjectInPool::OverlapBegin);
	}

	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}