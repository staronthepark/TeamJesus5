// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAreaActor.h"

AMonsterAreaActor::AMonsterAreaActor()
{
	Area = CreateDefaultSubobject<UBoxComponent>(TEXT("Area Box"));
	Area->SetupAttachment(RootComponent);
}

void AMonsterAreaActor::BeginPlay()
{
	Super::BeginPlay();

	Area->OnComponentBeginOverlap.AddDynamic(this, &AMonsterAreaActor::BeginOverlap);
	Area->OnComponentEndOverlap.AddDynamic(this, &AMonsterAreaActor::EndOverlap);

	AreaSize = Area->GetUnscaledBoxExtent();

	CheckMonster();
}

void AMonsterAreaActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterAreaActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AMonsterAreaActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (auto Monster : MonsterArr)
	{
		Monster->IsPatrol = true;
		Monster->MonsterController->FindPlayer = false;
		Monster->MonsterMoveEventIndex = 0;
	}
}

void AMonsterAreaActor::CheckMonster()
{
	TArray<FHitResult> OutHits;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepMultiByChannel(
		OUT OutHits,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel17,
		FCollisionShape::MakeBox(AreaSize),
		Params);

	FVector Center = GetActorLocation();
	FColor DrawColor;

	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	DrawDebugBox(GetWorld(), Center, AreaSize, DrawColor, false, 5.f);

	if (bResult)
	{
		for (auto HitActor : OutHits)
		{
			auto MonsterActor = Cast<AEnemyMonster>(HitActor.GetActor());
			MonsterArr.AddUnique(Cast<AEnemyMonster>(MonsterActor));
		}
	}
}

