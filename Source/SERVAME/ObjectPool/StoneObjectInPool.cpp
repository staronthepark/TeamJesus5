// Fill out your copyright notice in the Description page of Project Settings.


#include "StoneObjectInPool.h"
#include "..\Player\PlayerCharacter.h"

AStoneObjectInPool::AStoneObjectInPool()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	RootComponent = SceneComp;

	ProjectileCollision = CreateDefaultSubobject<UBoxComponent>("Collision");
	ProjectileCollision->SetupAttachment(RootComponent);
	ProjectileCollision->SetCollisionProfileName("AIProjectile");
}

void AStoneObjectInPool::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);

	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &AStoneObjectInPool::OnCollisionBeginOverlap);
	GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void AStoneObjectInPool::Tick(float DeltaTime)
{
	SetActorLocation(GetActorLocation() += MoveDir * Speed * DeltaTime);
}

void AStoneObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);

	SetActorTickEnabled(false);

	auto Player = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass()));

	MoveDir = FVector::ZeroVector;

	if (LifeTime > 0 && active)
	{
		GetWorldTimerManager().SetTimer(LifeTimer, this, &AStoneObjectInPool::ReturnObject, LifeTime);
	}
}

void AStoneObjectInPool::ReturnObject()
{
	Super::ReturnObject();
	SetActorTickEnabled(false);
}

void AStoneObjectInPool::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ReturnObject();

	if (OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this))
	{
	}
}
