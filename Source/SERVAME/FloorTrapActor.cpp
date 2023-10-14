// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTrapActor.h"

AFloorTrapActor::AFloorTrapActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("TrapMesh");
	SceneComp = CreateDefaultSubobject<UStaticMeshComponent>("SceneComp");
	MeshComp->SetupAttachment(SceneComp);
}

void AFloorTrapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActive)
	{
		MeshComp->SetRelativeLocation(FVector(0, 0, Speed * DeltaTime));
	}
}

void AFloorTrapActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFloorTrapActor::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();
}

void AFloorTrapActor::EndTriggerEvent()
{
	Super::EndTriggerEvent();
}

void AFloorTrapActor::EnableEvent()
{
	Super::EnableEvent();

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel18,
		FCollisionShape::MakeBox(FVector(10, 10, 10)),
		Params);

	FColor DrawColor;
	
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), 200.0f, 16, DrawColor, false, 2.f);

	if (bResult && HitResult.GetActor())
	{
		FDamageEvent DamageEvent;
		auto Player = Cast<ABaseCharacter>(HitResult.GetActor());

		if (Player != nullptr)
		{
			Player->TakeDamage(Damage, DamageEvent, nullptr, this);
		}
	}
}