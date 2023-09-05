#include "MonsterController.h"
#include "EnemyMonster.h"
#include "Knight/KinghtMonster.h"
#include <Kismet/KismetMathLibrary.h>

AMonsterController::AMonsterController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	Sight->SightRadius = 1000.f;
	Sight->LoseSightRadius = 1500.f;
	Sight->PeripheralVisionAngleDegrees = 45.f;

	AIPerceptionComponent->ConfigureSense(*Sight);
	AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());

	SetGenericTeamId(FGenericTeamId(1));
}

void AMonsterController::BeginPlay()
{
	Super::BeginPlay();
	
	AEnemyMonster* MonsterTemp = Cast<AEnemyMonster>(GetPawn());

	if (MonsterTemp)
	{
		Monster = MonsterTemp;
	}

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterController::OnTargetPerceptionUpdated_Delegate);

	//AJesusBoss* boss;
	//boss = Cast<AJesusBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), AJesusBoss::StaticClass()));
	//
	//Damage = boss->BossDataStruct.DamageList[BossAnimationType::RANGEATTACK];
}

void AMonsterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player != nullptr)
	{
		if (Player->PlayerDataStruct.CharacterHp > 0)
		{
			if (DetectedActorArr.Num() >= 1)
			{
				return;
			}

			AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISenseConfig_Sight::StaticClass(), DetectedActorArr);
		}
		else
		{
			DetectedActorArr.Empty();
		}
	}
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

void AMonsterController::Patrol(FVector Location, int PatrolArrNum)
{
	IsArrived = false;

	auto type = MoveToLocation(Location);

	FRotator ToTarget = UKismetMathLibrary::FindLookAtRotation(Monster->GetActorLocation(), Location);
	FRotator RealToTarget = FRotator(0.f, ToTarget.Yaw, 0.f);

	FRotator LookAtRotation = FMath::RInterpTo(Monster->GetActorRotation(), RealToTarget, GetWorld()->DeltaTimeSeconds, 3.f);
	Monster->SetActorRotation(LookAtRotation);

	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		IsArrived = true;
		Monster->PatrolIndexCount++;

		if (Monster->PatrolIndexCount >= PatrolArrNum)
			Monster->PatrolIndexCount = 0;
	}
}

void AMonsterController::OnTargetPerceptionUpdated_Delegate(AActor* Actor, FAIStimulus Stimulus)
{
	switch (Stimulus.Type)
	{
	case 0:
		//auto Dist = FVector::Distance(Player->GetActorLocation(), Monster->GetActorLocation());
		//
		//if (Dist <= 1400)
		//{
		//	if (GetTeamAttitudeTowards(*Actor) == ETeamAttitude::Hostile)
		//	{
		//		UE_LOG(LogTemp, Warning, TEXT("FindPlayer"));
		//		FindPlayer = true;
		//	}
		//}
		//else
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("LostPlayer"));
		//	FindPlayer = false;
		//}
		break;

	default:
		return;
	}
}

void AMonsterController::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	Player = Cast<APlayerCharacter>(Actor);

	if (Player == nullptr)
		return;

	UE_LOG(LogTemp, Log, TEXT("OnPerception"));
	SetFocus(Stimulus.WasSuccessfullySensed() ? Player : nullptr);

	if (Monster == nullptr)
	{
		Monster = Cast<AEnemyMonster>(GetPawn());
	}

	auto Dist = FVector::Distance(Player->GetActorLocation(), Monster->GetActorLocation());

	//퍼셉션 시야각 거리 - 100
	if (Dist <= PerceptionSight)
	{
		if (GetTeamAttitudeTowards(*Actor) == ETeamAttitude::Hostile)
		{
			UE_LOG(LogTemp, Warning, TEXT("FindPlayer"));
			FindPlayer = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LostPlayer"));
		FindPlayer = false;

		if (Monster->MyMonsterType == MonsterType::KNIGHT)
		{
			auto Knight = Cast<AKinghtMonster>(Monster);

			Knight->MonsterMoveEventIndex = 0;
			Knight->ChangeActionType(MonsterActionType::MOVE);
			Knight->KnightAnimInstance->BlendSpeed = Knight->WalkBlend;
		}
		else
		{
			Monster->ChangeMontageAnimation(MonsterAnimationType::IDLE);
			Monster->MonsterMoveEventIndex = 1;
		}
	}
}

void AMonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterController::OnPerception);
}

void AMonsterController::OnUnPossess()
{
	Super::OnUnPossess();
}

ETeamAttitude::Type AMonsterController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (auto OtherPawn = Cast<APawn>(&Other))
	{
		if (auto const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			auto ID = TeamAgent->GetGenericTeamId();
			auto intid = ID.GetId();

			UE_LOG(LogTemp, Warning, TEXT("ID : %d"), intid);

			if (ID == FGenericTeamId(1))
			{
				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile;
			}
		}
	}

	return ETeamAttitude::Hostile;
}

