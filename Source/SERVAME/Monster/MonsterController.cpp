#include "MonsterController.h"
#include "EnemyMonster.h"

AMonsterController::AMonsterController()
{
	
}

void AMonsterController::BeginPlay()
{
	Super::BeginPlay();

	AEnemyMonster* MonsterTemp = Cast<AEnemyMonster>(GetPawn());

	if (MonsterTemp)
	{
		Monster = MonsterTemp;
	}

	//AJesusBoss* boss;
	//boss = Cast<AJesusBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), AJesusBoss::StaticClass()));
	//
	//Damage = boss->BossDataStruct.DamageList[BossAnimationType::RANGEATTACK];
}

void AMonsterController::Movement(FVector Location)
{
	MoveToLocation(Location);
}

void AMonsterController::MoveWhenArrived(FVector Location)
{
	IsArrived = false;

	auto type =  MoveToLocation(Location);

	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		IsArrived = true;
		Monster->CircleIndexCount++;
	}
}
