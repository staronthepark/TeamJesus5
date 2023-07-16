// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "..\Boss_1\JesusBoss.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileObjectInPool.h"
#include "Kismet/KismetMathLibrary.h"

AProjectileObjectInPool::AProjectileObjectInPool()
{
	PrimaryActorTick.bCanEverTick = true;

	Collision1 = CreateDefaultSubobject<UBoxComponent>("Collision");
	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("Particle");	
	RootComponent = SceneComp;

	ParticleSystem->SetupAttachment(RootComponent);
	Collision1->SetupAttachment(RootComponent);
}

void AProjectileObjectInPool::BeginPlay()
{
	Super::BeginPlay();

	Collision1->OnComponentBeginOverlap.AddDynamic(this, &AProjectileObjectInPool::OnCollisionBeginOverlap);
	GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void AProjectileObjectInPool::ReturnObject()
{
	Super::ReturnObject();	
}


void AProjectileObjectInPool::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);

	if (ReturnWhenHit)
	{
		ReturnObject();
	}

	FVector HitDir = OtherActor->GetActorLocation() - GetActorLocation();
	FRotator Rotation = HitDir.Rotation();
	if (OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this))
	{		
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[20].ObjClass, OtherActor->GetActorLocation(), Rotation);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[21].ObjClass, OtherActor->GetActorLocation(), Rotation);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[22].ObjClass, OtherActor->GetActorLocation(), Rotation);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[0].ObjClass, OtherActor->GetActorLocation(), Rotation);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[1].ObjClass, OtherActor->GetActorLocation(), Rotation);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[3].ObjClass, OtherActor->GetActorLocation(), Rotation);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[1].ObjClass, OtherActor->GetActorLocation(), Rotation);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[31].ObjClass, OtherActor->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[5].ObjClass, OverlappedComponent->GetComponentLocation(), Rotation);
	}
}

void AProjectileObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);

	MoveDir = GetActorForwardVector();

//	ParticleSystem->SetWorldLocation(GetActorLocation());
//	ParticleSystem->SetActive(active, false);

	if (LifeTime > 0 && active)
	{
		GetWorldTimerManager().SetTimer(LifeTimer, this, &AProjectileObjectInPool::ReturnObject, LifeTime);
	}
}

void AProjectileObjectInPool::Tick(float DeltaTime)
{
	SetActorLocation(GetActorLocation() += MoveDir * Speed * DeltaTime);
}