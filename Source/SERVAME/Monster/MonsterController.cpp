#include "MonsterController.h"
#include "EnemyMonster.h"
#include "Knight/KinghtMonster.h"
#include <Kismet/KismetMathLibrary.h>

AMonsterController::AMonsterController()
{
	static ConstructorHelpers::FClassFinder<UBossUI> BossUIAsset(TEXT("/Game/02_Resource/04_UI/01_WBP/02_BossUI/WBP_BossUI"));

	if (BossUIAsset.Succeeded())
		BossUIClass = BossUIAsset.Class;

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
		if (Player->PlayerDataStruct.CharacterHp > 0 && !Monster->IsDie)
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

			if (IsUIActivate.Exchange(false))
			{
				if (IsValid(BossUI))
					BossUI->RemoveFromParent();
			}
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

void AMonsterController::MoveToStartLoc(FVector Location)
{
	auto type = MoveToLocation(Location);

	FRotator ToTarget = UKismetMathLibrary::FindLookAtRotation(Monster->GetActorLocation(), Location);
	FRotator RealToTarget = FRotator(0.f, ToTarget.Yaw, 0.f);

	FRotator LookAtRotation = FMath::RInterpTo(Monster->GetActorRotation(), RealToTarget, GetWorld()->DeltaTimeSeconds, 3.f);
	Monster->SetActorRotation(LookAtRotation);
	Monster->CanRotate = false;
	FindPlayer = false;

	auto Dist = FVector::Dist(Monster->GetActorLocation(), Location);

	if (type == EPathFollowingRequestResult::AlreadyAtGoal
		|| Dist <= 100.f)
	{
		Monster->CanRotate = true;

		if (Monster->MyMonsterType == MonsterType::KNIGHT ||
			Monster->MyMonsterType == MonsterType::ELITEKNIGHT ||
			Monster->MyMonsterType == MonsterType::DEADBODYOFKNIGHT ||
			Monster->MyMonsterType == MonsterType::PERSISTENTKNIGHT)
		{
			auto Knight = Cast<AKinghtMonster>(Monster);
			Knight->IsPatrol = false;
			Knight->WalkToRunBlend = false;
			Knight->isReturnBlend = true;
			Knight->MonsterMoveEventIndex = 1;
			Knight->ChangeActionType(MonsterActionType::NONE);
		}
		else
		{
			Monster->ChangeActionType(MonsterActionType::NONE);
			Monster->ChangeMontageAnimation(MonsterAnimationType::IDLE);
			Monster->TracePlayer = false;
			Monster->MonsterMoveEventIndex = 1;
		}
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
	if (!CanPerception)
		return;

	Player = Cast<APlayerCharacter>(Actor);

	if (Player == nullptr)
		return;

	if (Monster == nullptr)
	{
		Monster = Cast<AEnemyMonster>(GetPawn());
	}

	if (Monster->MonsterDataStruct.CharacterHp <= 0 || Monster->IsDie)
		return;

	SetFocus(Stimulus.WasSuccessfullySensed() ? Player : nullptr);
	
	auto Dist = FVector::Distance(Player->GetActorLocation(), Monster->GetActorLocation());
	Monster->PlayerCharacter = Player;

	if (Stimulus.WasSuccessfullySensed())
	{
		if (GetTeamAttitudeTowards(*Actor) == ETeamAttitude::Hostile)
		{
			UE_LOG(LogTemp, Warning, TEXT("FindPlayer: %s"), *Monster->GetName());
			FindPlayer = true;
			Monster->TracePlayer = true;

			if (Monster->MyMonsterType == MonsterType::NUN && Monster->IsBoss)
			{
				BossUI->AddToViewport();
				Monster->PlayerCharacter->UserSettingUI->WBP_UserSetting_GameUI->WBP_Language_Button->LeftButton->OnClicked.AddDynamic(this, &AMonsterController::ChangeLanguage);
				Monster->PlayerCharacter->UserSettingUI->WBP_UserSetting_GameUI->WBP_Language_Button->RightButton->OnClicked.AddDynamic(this, &AMonsterController::ChangeLanguage);
				BossUI->PlayBossHPOpenAnimation(true, EBossSettings::isahel);
			}
			else if (Monster->MyMonsterType == MonsterType::ELITEKNIGHT && Monster->IsBoss)
			{
				FindPlayer = false;
				Monster->TracePlayer = false;
				
				BossUI->AddToViewport();
				Monster->PlayerCharacter->UserSettingUI->WBP_UserSetting_GameUI->WBP_Language_Button->LeftButton->OnClicked.AddDynamic(this, &AMonsterController::ChangeLanguage);
				Monster->PlayerCharacter->UserSettingUI->WBP_UserSetting_GameUI->WBP_Language_Button->RightButton->OnClicked.AddDynamic(this, &AMonsterController::ChangeLanguage);
				Monster->ChangeMontageAnimation(MonsterAnimationType::ELITEKNIGHT_START);
				BossUI->PlayBossHPOpenAnimation(true, EBossSettings::paladin);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LostPlayer : %s"), *Monster->GetName());
		FindPlayer = false;

		if (Monster->MyMonsterType == MonsterType::NUN)
		{
			Monster->ChangeMontageAnimation(MonsterAnimationType::IDLE);
			Monster->MonsterMoveEventIndex = 1;
			if (IsValid(BossUI) && Monster->IsBoss)
			{
				BossUI->RemoveFromParent();
			}
		}
		else if (Monster->MyMonsterType == MonsterType::ELITEKNIGHT)
		{
			if(Monster->IsBoss)
				BossUI->RemoveFromParent();

			auto Knight = Cast<AKinghtMonster>(Monster);

			Knight->IsPatrol = true;
			Knight->CanRotate = false;
			Knight->KnightAnimInstance->BlendSpeed = Knight->WalkBlend;
			Knight->WalkToRunBlend = true;
			Knight->TracePlayer = false;
			Knight->MonsterMoveEventIndex = 0;
			Knight->ChangeActionType(MonsterActionType::MOVE);
		}
		else
		{
			auto Knight = Cast<AKinghtMonster>(Monster);

			if(Knight != nullptr)
			{
				if (Knight->IsSpawn)
					return;

				if (Monster->MyMonsterType == MonsterType::KNIGHT ||
					Monster->MyMonsterType == MonsterType::PERSISTENTKNIGHT)
				{
					Knight->IsPatrol = true;
					Knight->KnightAnimInstance->BlendSpeed = Knight->WalkBlend;
					Knight->WalkToRunBlend = true;
					Knight->TracePlayer = false;
					Knight->MonsterMoveEventIndex = 0;
					Knight->ChangeActionType(MonsterActionType::MOVE);
				}
				else if (Monster->MyMonsterType == MonsterType::DEADBODYOFKNIGHT)
				{
					Knight->IsPatrol = true;
					Knight->WalkToRunBlend = false;
					Knight->TracePlayer = false;
					Knight->MonsterMoveEventIndex = 0;
					Knight->ChangeActionType(MonsterActionType::MOVE);
				}
			}
			else
			{
				Monster->MonsterMoveEventIndex = 0;
				Monster->ChangeActionType(MonsterActionType::MOVE);
				Monster->TracePlayer = false;
			}
		}
	}
}

void AMonsterController::ChangeLanguage()
{
	BossUI->ChangeLanguage();
}

void AMonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterController::OnPerception);

	if (IsValid(BossUIClass))
	{
		BossUI = Cast<UBossUI>(CreateWidget(GetWorld(), BossUIClass));
		BossUI->SetHP(1);
	}
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

			//UE_LOG(LogTemp, Warning, TEXT("ID : %d"), intid);

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

