#include "MonsterController.h"
#include "EnemyMonster.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"

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

void AMonsterController::OnTargetPerceptionUpdated_Delegate(AActor* Actor, FAIStimulus Stimulus)
{
	switch (Stimulus.Type)
	{
	case 0:
		if (GetTeamAttitudeTowards(*Actor) == ETeamAttitude::Hostile)
		{
			UE_LOG(LogTemp, Warning, TEXT("FindPlayer"));
			FindPlayer = true;
		}
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

