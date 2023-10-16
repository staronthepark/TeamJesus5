// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadBodyOfKinght.h"
#include "..\KnightAttackTriggerComp.h"
#include "GameFramework/CharacterMovementComponent.h"

ADeadBodyOfKinght::ADeadBodyOfKinght()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(GetMesh());
	Trigger->SetCollisionProfileName("WorldTriggerBox");

	AnimTypeToStateType.Add(MonsterAnimationType::STARTDEAD, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::REVIVE, MonsterStateType::CANTACT);

	MonsterTickEventMap.Add(MonsterActionType::NONE, [&]()
		{
			if (MonsterController->FindPlayer && !Reviving)
				RotateMap[PlayerCharacter != nullptr]();
		});

	SetActionByRandomMap.Add(MonsterAnimationType::ATTACK1, [&](float percent)
		{
			if (percent <= 0.5f)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::ATTACK1);
			}
			else
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::POWERATTACK1);
			}
		});

	MonsterTickEventMap.Add(MonsterActionType::MOVE, [&]()
		{
			StateType = AnimTypeToStateType[MonsterAnimationType::IDLE];

			//기사시체에만 적용?
			if (CurrentDistance < AttackRange && MonsterController->FindPlayer)
				StartAttackTrigger(AttackAnimationType);

			if (CurrentDistance >= RunableDistance && !IsPatrol)
			{
				ChangeActionType(MonsterActionType::RUN);
			}
			else
			{
				if (!IsMoveStart)
					MinWalkTime = GetRandNum(3, 4);

				IsMoveStart = true;
				Temp = 0.f;
				CalcedDist = 0.f;
				InterpolationTime = 0.f;
				WalkToRunBlend = true;

				GetCharacterMovement()->MaxWalkSpeed = MonsterDataStruct.CharacterOriginSpeed;
				KnightAnimInstance->BlendSpeed = WalkBlend;
				RotateMap[PlayerCharacter != nullptr]();
				MonsterMoveMap[MonsterMoveEventIndex]();
			}
		});

	MontageEndEventMap.Add(MonsterAnimationType::DEAD, [&]()
		{
			KnightAnimInstance->PauseAnimation(MontageMap[AnimationType]);
		});

	MontageEndEventMap.Add(MonsterAnimationType::STARTDEAD, [&]()
		{	
			ChangeMontageAnimation(MonsterAnimationType::STARTDEAD);
		});

	MontageEndEventMap.Add(MonsterAnimationType::REVIVE, [&]()
		{
			StopAnimMontage(MontageMap[AnimationType]);
			Imotal = false;
			Reviving = false;
			StateType = MonsterStateType::NONE;
			HitCollision->Activate();
			AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			TracePlayer = true;

			//ActivateHpBar();
			ActivateHitCollision();
			ActivateSMOverlap();
			ParryingCollision1->Activate();

			PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

			MonsterMoveEventIndex = 1;
			ChangeActionType(MonsterActionType::MOVE);
			KnightAnimInstance->BlendSpeed = WalkBlend;
		});
}

void ADeadBodyOfKinght::BeginPlay()
{
	Super::BeginPlay();

	MyMonsterType = MonsterType::DEADBODYOFKNIGHT;
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ADeadBodyOfKinght::OnTriggerBegin);

	//TODO : dead 몽타주 정지 시켜놓고 쓰러진 상태에서 시작.
	AnimationType = MonsterAnimationType::STARTDEAD;
	ChangeActionType(MonsterActionType::NONE);
	ChangeMontageAnimation(MonsterAnimationType::STARTDEAD);

	StateType = MonsterStateType::CANTACT;
	Imotal = true;
	TracePlayer = false;
	StateType = MonsterStateType::NONE;
	HitCollision->Deactivate();
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADeadBodyOfKinght::Tick(float DeltaTime)
{
	if (Reviving)
		return;

	Super::Tick(DeltaTime);
}

void ADeadBodyOfKinght::RespawnCharacter()
{
	Super::RespawnCharacter();

	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StateType = MonsterStateType::CANTACT;
	Imotal = true;
	TracePlayer = false;
	StateType = MonsterStateType::NONE;
	HitCollision->Deactivate();
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADeadBodyOfKinght::OnTriggerBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ChangeActionType(MonsterActionType::NONE);
	ChangeMontageAnimation(MonsterAnimationType::REVIVE);
	Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

