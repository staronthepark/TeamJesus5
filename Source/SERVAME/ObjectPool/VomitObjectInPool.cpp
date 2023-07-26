// Fill out your copyright notice in the Description page of Project Settings.


#include "VomitObjectInPool.h"

AVomitObjectInPool::AVomitObjectInPool()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("RootSceneComp");
	RootComponent = RootComp;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("HitCollision");
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionProfileName("AIProjectile");

	GroundHitCollision = CreateDefaultSubobject<USphereComponent>("GroundHitCollision");
	GroundHitCollision->SetupAttachment(RootComponent);

	ProjectileEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Vomit Effect"));
	ProjectileEffect->SetupAttachment(RootComponent);

	BurstEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Vomit Burst Effect"));
	BurstEffect->SetupAttachment(RootComponent);

	ProjectileMoveMentComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMoveMentComponent->SetUpdatedComponent(RootComp);
	ProjectileMoveMentComponent->InitialSpeed = 3000.0f;
	ProjectileMoveMentComponent->MaxSpeed = 3000.0f;
	ProjectileMoveMentComponent->bRotationFollowsVelocity = true;
	ProjectileMoveMentComponent->bShouldBounce = true;
	ProjectileMoveMentComponent->Bounciness = 0.3f;
}

void AVomitObjectInPool::SetActive(bool active)
{
	FVector outVelocity = FVector::ZeroVector;   // ��� Velocity
	
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, outVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), arcValue))
	{
		FPredictProjectilePathParams predictParams(20.0f, startLoc, outVelocity, 15.0f);   // 20: tracing ������ ������Ÿ�� ũ��, 15: �ù����̼ǵǴ� Max �ð�(��)
		predictParams.DrawDebugTime = 15.0f;     //����� ���� �������� �ð� (��)
		predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime �� �����ϸ� EDrawDebugTrace::Type::ForDuration �ʿ�.
		predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
		FPredictProjectilePathResult result;
		UGameplayStatics::PredictProjectilePath(this, predictParams, result);

		//objectToSend->AddImpluse(outVelocity); // objectToSend�� �߻�ü
	}
}


void AVomitObjectInPool::BeginPlay()
{
	Super::BeginPlay();

}

void AVomitObjectInPool::Tick(float DeltaTime)
{
}

void AVomitObjectInPool::ReturnObject()
{
	Super::ReturnObject();

}

void AVomitObjectInPool::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AVomitObjectInPool::OnGroundOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
