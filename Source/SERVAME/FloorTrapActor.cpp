// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTrapActor.h"

AFloorTrapActor::AFloorTrapActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("TrapMesh");
	SceneComp = CreateDefaultSubobject<UStaticMeshComponent>("SceneComp");
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	MeshComp->SetupAttachment(SceneComp);
	BoxComp->SetupAttachment(SceneComp);
}

void AFloorTrapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTrigger)
	{
		if (IsPlaced)
		{
			if (MeshComp->GetComponentLocation().Z < 0)
			{
				float Z = MeshComp->GetComponentLocation().Z;
				MeshComp->SetRelativeLocation(FVector(0, 0, Z -= Speed * DeltaTime));
			}
			else
			{
				IsPlaced = false;
				BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
		else
		{
			if (MeshComp->GetComponentLocation().Z > -129.705314)
			{
				float Z = MeshComp->GetComponentLocation().Z;
				MeshComp->SetRelativeLocation(FVector(0, 0, Z -= Speed * DeltaTime));
			}
			else
			{
				IsPlaced = true;
				IsTrigger = false;
			}
		}
	}
}

void AFloorTrapActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFloorTrapActor::OnBeginOverlap);

	if (IsTimer)
	{
		GetWorldTimerManager().SetTimer(Timer, this, &AFloorTrapActor::EnableTrap, Time);
	}

	IsPlaced = true;
	IsActive = false;
}

void AFloorTrapActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	FDamageEvent DamageEvent;
	auto Player = Cast<ABaseCharacter>(OtherActor);

	if (Player != nullptr)
	{
		Player->TakeDamage(Damage, DamageEvent, nullptr, this);
	}
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFloorTrapActor::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();

	IsTrigger = true;
}

void AFloorTrapActor::EndTriggerEvent()
{
	Super::EndTriggerEvent();
}

void AFloorTrapActor::EnableEvent()
{
	Super::EnableEvent();

	if(!IsTimer && IsPlaced)
	EnableTrap();
}

void AFloorTrapActor::EnableTrap()
{
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	if (IsTimer)
	{
		IsPlaced = true;
		GetWorldTimerManager().SetTimer(Timer, this, &AFloorTrapActor::EnableTrap, Time);
	}
}