// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTrapActor.h"

AFloorTrapActor::AFloorTrapActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("TrapMesh");
	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	MeshComp->SetupAttachment(SceneComp);
	BoxComp->SetupAttachment(SceneComp);
}

void AFloorTrapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsTrigger)
	{
		if (IsEnable)
		{
			if (MeshComp->GetRelativeLocation().Z < 0)
			{
				MeshComp->SetRelativeLocation(FVector(0, 0, MeshComp->GetRelativeLocation().Z + Speed * DeltaTime));
			}
			else
			{
				IsEnable = false;
			}
		}
		else if (MeshComp->GetRelativeLocation().Z > -129.705314)
		{
			MeshComp->SetRelativeLocation(FVector(0, 0, MeshComp->GetRelativeLocation().Z - Speed * 0.1f * DeltaTime));
			BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			if(MeshComp->GetRelativeLocation().Z <= -129.705314)
			IsTrigger = false;
		}
	}
}

void AFloorTrapActor::BeginPlay()
{
	Super::BeginPlay();

	IsTrigger = false;
	IsEnable = false;
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFloorTrapActor::OnBeginOverlap);

	if (IsTimer)
	{
		GetWorldTimerManager().SetTimer(Timer, this, &AFloorTrapActor::EnableTrap, Time);
	}
}

void AFloorTrapActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	FDamageEvent DamageEvent;
	auto Player = Cast<ABaseCharacter>(OtherActor);

	if (Player != nullptr)
	{
		AObjectPool& objectpool = AObjectPool::GetInstance();
		objectpool.SpawnObject(objectpool.ObjectArray[6].ObjClass, Player->GetActorLocation(), FRotator::ZeroRotator);
		objectpool.SpawnObject(objectpool.ObjectArray[31].ObjClass, OtherActor->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
		objectpool.SpawnObject(objectpool.ObjectArray[5].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator::ZeroRotator);
		Player->TakeDamage(Damage, DamageEvent, nullptr, this);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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
	if (IsTrigger)return;
	Super::EnableEvent();
	EnableTrap();
}

void AFloorTrapActor::EnableTrap()
{
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	IsTrigger = true;
	IsEnable = true;
	AObjectPool& objectpool = AObjectPool::GetInstance();
	objectpool.SpawnObject(objectpool.ObjectArray[13].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
}