// Fill out your copyright notice in the Description page of Project Settings.


#include "NunEffectObjInPool.h"

ANunEffectObjInPool::ANunEffectObjInPool()
{
	RootComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	RootComponent = RootComp;

	CurrentEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CurrentEffect"));
	CurrentEffect->SetupAttachment(RootComponent);

	DarkProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DarkProjectileCollision"));
	DarkProjectileCollision->SetupAttachment(RootComponent);

	FogAttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("FogAttackCollision"));
	FogAttackCollision->SetupAttachment(RootComponent);
}

void ANunEffectObjInPool::BeginPlay()
{
	DarkProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ANunEffectObjInPool::OnDarkprojectileBeginOverlap);
	DarkProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ANunEffectObjInPool::OnFogBeginOverlap);

	CurrentEffect->SetAsset(GetTypeEffect[Type]);
}

void ANunEffectObjInPool::Tick(float DeltaTime)
{
}

void ANunEffectObjInPool::SetActive(bool active)
{
	auto temp = GetTypeEffect[Type];
	
}

void ANunEffectObjInPool::ReturnObject()
{
}

void ANunEffectObjInPool::OnDarkprojectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ANunEffectObjInPool::OnFogBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
