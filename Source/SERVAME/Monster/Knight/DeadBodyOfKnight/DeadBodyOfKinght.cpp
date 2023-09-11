// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadBodyOfKinght.h"
#include "..\KnightAttackTriggerComp.h"

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
			ChangeActionType(MonsterActionType::ATTACK);
			ChangeMontageAnimation(MonsterAnimationType::ATTACK1);
		});

	MontageEndEventMap.Add(MonsterAnimationType::DEAD, [&]()
		{
			//ChangeMontageAnimation(MonsterAnimationType::DEADLOOP);
			//IsStun = true;
			//CanExecution = true;
		});

	MontageEndEventMap.Add(MonsterAnimationType::STARTDEAD, [&]()
		{	
			ChangeMontageAnimation(MonsterAnimationType::STARTDEAD);
		});

	MontageEndEventMap.Add(MonsterAnimationType::REVIVE, [&]()
		{
			Imotal = false;
			Reviving = false;
			StateType = MonsterStateType::NONE;
			HitCollision->Activate();
			AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			TracePlayer = true;

			ActivateHpBar();
			ActivateHitCollision();
			ActivateSMOverlap();
			ActivateRightWeapon();
			ParryingCollision1->Activate();

			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
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

void ADeadBodyOfKinght::OnTriggerBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ChangeActionType(MonsterActionType::NONE);
	ChangeMontageAnimation(MonsterAnimationType::REVIVE);
}

