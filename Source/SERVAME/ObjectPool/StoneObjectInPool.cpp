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
}

void AStoneObjectInPool::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);
	BurstEffect->Deactivate();

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
	BurstEffect->Activate();

	AJesusBoss2* boss;
	boss = Cast<AJesusBoss2>(UGameplayStatics::GetActorOfClass(GetWorld(), AJesusBoss2::StaticClass()));

	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherComp->GetName());

	if (OtherComp->GetName() == "ShieldCollision")
	{
		UE_LOG(LogTemp, Warning, TEXT("shield"));
		auto Player = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass()));
		Player->SetShieldHP(-Damage);
		boss->CameraShake(boss->PlayerCameraShake);
		boss->VibrateGamePad(0.4f, 0.4f);
		ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		return;
	}
	else if (OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this))
	{
		UE_LOG(LogTemp, Warning, TEXT("take damage"));
	}
}
