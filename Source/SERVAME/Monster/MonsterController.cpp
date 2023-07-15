#include "MonsterController.h"

AMonsterController::AMonsterController()
{
	
}

void AMonsterController::BeginPlay()
{
	Super::BeginPlay();


	//AJesusBoss* boss;
	//boss = Cast<AJesusBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), AJesusBoss::StaticClass()));
	//
	//Damage = boss->BossDataStruct.DamageList[BossAnimationType::RANGEATTACK];
}

void AMonsterController::Movement(FVector Location)
{
	MoveToLocation(Location);
}