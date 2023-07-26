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

	//ProjectileMoveMentComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	//ProjectileMoveMentComponent->SetUpdatedComponent(SphereCollision);
	//ProjectileMoveMentComponent->InitialSpeed = 3000.0f;
	//ProjectileMoveMentComponent->MaxSpeed = 3000.0f;
	//ProjectileMoveMentComponent->bRotationFollowsVelocity = true;
	//ProjectileMoveMentComponent->bShouldBounce = true;
	//ProjectileMoveMentComponent->Bounciness = 0.3f;
}

void AVomitObjectInPool::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	ProjectileEffect->Deactivate();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AVomitObjectInPool::OnCollisionBeginOverlap);
	GroundHitCollision->OnComponentBeginOverlap.AddDynamic(this, &AVomitObjectInPool::OnGroundOverlap);
}

void AVomitObjectInPool::Tick(float DeltaTime)
{

}

void AVomitObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);

	BurstEffect->Deactivate();

	if (active && LifeTime > 0)
	{
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
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AVomitObjectInPool::OnGroundOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ProjectileEffect->Deactivate();
	BurstEffect->Activate();

	IsHitGround = true;
	SetActorTickEnabled(false);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
