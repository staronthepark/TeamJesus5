// Fill out your copyright notice in the Description page of Project Settings.

#include "StoneObjectInPool.h"
#include "..\Player\PlayerCharacter.h"
#include "..\Boss_2\JesusBoss2.h"

AStoneObjectInPool::AStoneObjectInPool()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	RootComponent = SceneComp;

	ProjectileCollision = CreateDefaultSubobject<UBoxComponent>("Collision");
	ProjectileCollision->SetupAttachment(RootComponent);
	ProjectileCollision->SetCollisionProfileName("AIProjectile");

	BurstEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Stone Burst Effect"));
	BurstEffect->SetupAttachment(RootComponent);

	StoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stone Mesh"));
	StoneMesh->SetupAttachment(RootComponent);
}

void AStoneObjectInPool::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);
	BurstEffect->Deactivate();

	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &AStoneObjectInPool::OnCollisionBeginOverlap);
	GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	Boss2 = Cast<AJesusBoss2>(UGameplayStatics::GetActorOfClass(GetWorld(), AJesusBoss2::StaticClass()));
}

void AStoneObjectInPool::Tick(float DeltaTime)
{
	SetActorLocation(GetActorLocation() += MoveDir * Speed * DeltaTime);
}

void AStoneObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);
	StoneMesh->SetActive(true);
	SetActorTickEnabled(false);
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
	BurstEffect->Activate();
	StoneMesh->SetActive(false);
	Boss2->PlayMonsterSoundInPool(EMonsterAudioType::BOSS2_ROCKHIT);

	if (OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this))
	{
	}
}
