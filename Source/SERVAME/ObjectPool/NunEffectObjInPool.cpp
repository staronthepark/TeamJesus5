#include "NunEffectObjInPool.h"
#include "NunEffectObjInPool.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "NunEffectObjInPool.h"

ANunEffectObjInPool::ANunEffectObjInPool()
{
	RootComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	RootComponent = RootComp;

	CurrentEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CurrentEffect"));
	CurrentEffect->SetupAttachment(RootComponent);

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	ProjectileCollision->SetupAttachment(RootComponent);

	RangeAttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RangeAttackCollision"));
	RangeAttackCollision->SetupAttachment(RootComponent);
}

void ANunEffectObjInPool::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RangeAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ANunEffectObjInPool::OnProjectileBeginOverlap);
	RangeAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ANunEffectObjInPool::OnRangeAttackBeginOverlap);
}

void ANunEffectObjInPool::Tick(float DeltaTime)
{
	if (IsShot)
		SetActorLocation(GetActorLocation() += MoveDir * Speed * DeltaTime);
}

void ANunEffectObjInPool::SetActive(bool active)
{
	Super::SetActive(active);

	MoveDir = FVector::ZeroVector;

	if (LifeTime > 0 && active)
		GetWorldTimerManager().SetTimer(LifeTimer, this, &ANunEffectObjInPool::ReturnObject, LifeTime);
}

void ANunEffectObjInPool::ReturnObject()
{
	Super::ReturnObject();
	SetActorTickEnabled(false);
}

void ANunEffectObjInPool::ShotProjectile()
{
	IsShot = true;
}

void ANunEffectObjInPool::ActivateCurrentEffect()
{
	CurrentEffect->Activate();
}

void ANunEffectObjInPool::DeactivateCurrentEffect()
{
	CurrentEffect->Deactivate();
}

void ANunEffectObjInPool::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CurrentEffect != nullptr)
		CurrentEffect->Activate();

	if (OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this))
	{

	}
}

void ANunEffectObjInPool::OnRangeAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
