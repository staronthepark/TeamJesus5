// Fill out your copyright notice in the Description page of Project Settings.


#include "VomitObjectInPool.h"

AVomitObjectInPool::AVomitObjectInPool()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("HitCollision");
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionProfileName("Vomit");
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = SphereCollision;

	GroundHitCollision = CreateDefaultSubobject<USphereComponent>("GroundHitCollision");
	GroundHitCollision->SetupAttachment(SphereCollision);
	GroundHitCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ProjectileEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Vomit Effect"));
	ProjectileEffect->SetupAttachment(SphereCollision);

	BurstEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Vomit Burst Effect"));
	BurstEffect->SetupAttachment(SphereCollision);

	SpawnEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Vomit Spawn Effect"));
	SpawnEffect->SetupAttachment(SphereCollision);

	DispersionEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Vomit Dispersion Effect"));
	DispersionEffect->SetupAttachment(SphereCollision);
}

void AVomitObjectInPool::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	ProjectileEffect->Deactivate();
	BurstEffect->Deactivate();
	DispersionEffect->Deactivate();
	SpawnEffect->Deactivate();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AVomitObjectInPool::OnCollisionBeginOverlap);
	SphereCollision->SetEnableGravity(false);
	GroundHitCollision->OnComponentBeginOverlap.AddDynamic(this, &AVomitObjectInPool::OnGroundOverlap);
}

void AVomitObjectInPool::Tick(float DeltaTime)
{

}

void AVomitObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);
	
	if (active && LifeTime > 0)
	{
		SphereCollision->SetEnableGravity(true);
		GetWorldTimerManager().SetTimer(LifeTimer, this, &AVomitObjectInPool::ReturnObject, LifeTime);
	}
}

void AVomitObjectInPool::ReturnObject()
{
	Super::ReturnObject();
	SetActorTickEnabled(false);
}

void AVomitObjectInPool::ShootProjectile(FVector target)
{
	SpawnEffect->DeactivateImmediate();

	targetLoc = target;
	startLoc = SphereCollision->GetComponentLocation();
	FVector outVelocity = FVector::ZeroVector;   // ��� Velocity

	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(SphereCollision, outVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), arcValue))
	{
		if (debugLine)
		{
			outVelocity *= Speed;
			FPredictProjectilePathParams predictParams(20.0f, startLoc, outVelocity, 15.0f);   // 20: tracing ������ ������Ÿ�� ũ��, 15: �ù����̼ǵǴ� Max �ð�(��)
			predictParams.DrawDebugTime = 15.0f;     //����� ���� �������� �ð� (��)
			predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime �� �����ϸ� EDrawDebugTrace::Type::ForDuration �ʿ�.
			predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
			FPredictProjectilePathResult result;
			UGameplayStatics::PredictProjectilePath(SphereCollision, predictParams, result);
		}

		SphereCollision->AddImpulse(outVelocity);
	}
}

void AVomitObjectInPool::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);

	if (OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this))
	{
		ProjectileEffect->Deactivate();
		BurstEffect->Activate();
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AVomitObjectInPool::OnGroundOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("????"));

	BurstEffect->Activate();
	ProjectileEffect->Deactivate();

	IsHitGround = true;
	SetActorTickEnabled(false);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
