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
	SetActorTickEnabled(false);
	Super::ReturnObject();	
}


void AProjectileObjectInPool::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);

	if (ReturnWhenHit)
	{
		ReturnObject();
		GetWorldTimerManager().ClearTimer(LifeTimer);
		SetActorTickEnabled(false);
	}

	AJesusBoss* boss;
	boss = Cast<AJesusBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), AJesusBoss::StaticClass()));

	if (OtherComp->GetName() == "ShieldCollision")
	{
		auto Player = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass()));
		Player->SetShieldHP(-Damage, GetActorLocation());
		boss->CameraShake(boss->PlayerCameraShake);
		boss->VibrateGamePad(0.4f, 0.4f);
		Collision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		return;
	}

	FVector HitDir = OtherActor->GetActorLocation() - GetActorLocation();
	FRotator Rotation = HitDir.Rotation();
	if (OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this))
	{		
		AObjectPool & objectpool = AObjectPool::GetInstance();

		objectpool.SpawnObject(objectpool.ObjectArray[6].ObjClass, OtherActor->GetActorLocation(), FRotator::ZeroRotator);
		objectpool.SpawnObject(objectpool.ObjectArray[31].ObjClass, OtherActor->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
		objectpool.SpawnObject(objectpool.ObjectArray[20].ObjClass, OtherActor->GetActorLocation(), Rotation);
		objectpool.SpawnObject(objectpool.ObjectArray[21].ObjClass, OtherActor->GetActorLocation(), Rotation);
		objectpool.SpawnObject(objectpool.ObjectArray[22].ObjClass, OtherActor->GetActorLocation(), Rotation);	
		objectpool.SpawnObject(objectpool.ObjectArray[5].ObjClass, OverlappedComponent->GetComponentLocation(), Rotation);
	}
}

void AProjectileObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);

	MoveDir = GetActorForwardVector();
	if (LifeTime > 0 && active)
	{
		GetWorldTimerManager().SetTimer(LifeTimer, this, &AProjectileObjectInPool::ReturnObject, LifeTime);
	}
}

void AProjectileObjectInPool::Tick(float DeltaTime)
{
	SetActorLocation(GetActorLocation() += MoveDir * Speed * DeltaTime);
}