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
	FVector outVelocity = FVector::ZeroVector;   // 결과 Velocity
	
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, outVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), arcValue))
	{
		FPredictProjectilePathParams predictParams(20.0f, startLoc, outVelocity, 15.0f);   // 20: tracing 보여질 프로젝타일 크기, 15: 시물레이션되는 Max 시간(초)
		predictParams.DrawDebugTime = 15.0f;     //디버그 라인 보여지는 시간 (초)
		predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime 을 지정하면 EDrawDebugTrace::Type::ForDuration 필요.
		predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
		FPredictProjectilePathResult result;
		UGameplayStatics::PredictProjectilePath(this, predictParams, result);

		//objectToSend->AddImpluse(outVelocity); // objectToSend는 발사체
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
