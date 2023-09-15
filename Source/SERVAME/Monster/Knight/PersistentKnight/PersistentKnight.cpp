
// Fill out your copyright notice in the Description page of Project Settings.


#include "PersistentKnight.h"
#include "..\KnightAttackTriggerComp.h"
#include "KnightHeadAnimInstance.h"

APersistentKnight::APersistentKnight()
{
	KnightHeadSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("KnightHeadSkeletalMesh"));
	KnightHeadSkeletalMesh->SetupAttachment(GetMesh(), FName("Bip001-Head"));

	AnimTypeToStateType.Add(MonsterAnimationType::REVIVE, MonsterStateType::CANTACT);

	MonsterTickEventMap.Add(MonsterActionType::NONE, [&]()
		{
			if (MonsterController->FindPlayer && !Reviving)
				RotateMap[PlayerCharacter != nullptr]();
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::REVIVE, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::REVIVE].Add(true, [&]()
		{
		});
	NotifyBeginEndEventMap[MonsterAnimationType::REVIVE].Add(false, [&]()
		{
		});

	MontageEndEventMap.Add(MonsterAnimationType::DEAD, [&]()
		{
			if (IsFirstDie)
			{
				IsFirstDie = false;
				FirstDie();
			}
			else
			{
				//ChangeMontageAnimation(MonsterAnimationType::DEADLOOP);
				//IsStun = true;
				//CanExecution = true;
			}
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

	SetActionByRandomMap.Add(MonsterAnimationType::ATTACK1, [&](float percent)
		{
			ChangeActionType(MonsterActionType::ATTACK);
			ChangeMontageAnimation(MonsterAnimationType::ATTACK1);
		});
}

void APersistentKnight::BeginPlay()
{
	Super::BeginPlay();

	MyMonsterType = MonsterType::PERSISTENTKNIGHT;
}

void APersistentKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float APersistentKnight::Die(float Dm)
{
	if (MonsterDataStruct.CharacterHp <= 0)
	{
		if (IsFirstDie)
		{	
			auto HeadAnimInstance = Cast<UKnightHeadAnimInstance>(KnightHeadSkeletalMesh->GetAnimInstance());
			HeadAnimInstance->Rot = RotVal;

			//머리 날리기
			KnightHeadSkeletalMesh->SetCollisionProfileName("Ragdoll");
			KnightHeadSkeletalMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			KnightHeadSkeletalMesh->AddImpulseToAllBodiesBelow(AddVec * Strength/*, FName(TEXT("B_Helmet_spin")*/);
			KnightHeadSkeletalMesh->AddImpulseToAllBodiesBelow(FVector(-20,0,0) * Strength, FName(TEXT("B_Helmet_spin")));
			KnightHeadSkeletalMesh->SetSimulatePhysics(true);
			KnightHeadSkeletalMesh->SetEnableGravity(true);

			Imotal = true;
			DeactivateHpBar();
			DeactivateHitCollision();

			KnightAnimInstance->StopMontage(MontageMap[AnimationType]);
			ChangeActionType(MonsterActionType::DEAD);
			StateType = MonsterStateType::CANTACT;

			MonsterController->StopMovement();
			DeactivateSMOverlap();
			ParryingCollision1->Deactivate();
			DeactivateRightWeapon();
			ChangeMontageAnimation(MonsterAnimationType::DEAD);

			return Dm;
		}
		else
		{
			Super::Die(Dm);

			return Dm;
		}
	}

	return Dm;
}

void APersistentKnight::FirstDie()
{
	Imotal = true;
	Reviving = true;

	MonsterDataStruct.CharacterHp = (MonsterDataStruct.CharacterMaxHp / 3.f);
	float CurrentPercent = MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp;
	MonsterHPWidget->SetHP(CurrentPercent);

	//살아나는 애니 재생
	StateType = MonsterStateType::CANTACT;
	HitCollision->Deactivate();
	ChangeActionType(MonsterActionType::NONE);
	ChangeMontageAnimation(MonsterAnimationType::REVIVE);
}
