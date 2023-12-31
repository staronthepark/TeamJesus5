
// Fill out your copyright notice in the Description page of Project Settings.


#include "PersistentKnight.h"
#include "..\KnightAttackTriggerComp.h"
#include "KnightHeadAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

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
				KnightAnimInstance->PauseAnimation(MontageMap[AnimationType]);
			}
		});

	MontageEndEventMap.Add(MonsterAnimationType::REVIVE, [&]()
		{
			if (MonsterController->FindPlayer)
			{
				IsDie = false;
				Imotal = false;
				Reviving = false;
				StateType = MonsterStateType::NONE;
				HitCollision->Activate();
				AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				TracePlayer = true;

				ActivateHitCollision();

				ChangeActionType(MonsterActionType::NONE);
				ChangeMontageAnimation(MonsterAnimationType::IDLE);
			}
			else
			{
				IsDie = false;
				Imotal = false;
				Reviving = false;
				StateType = MonsterStateType::NONE;
				HitCollision->Activate();
				AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				IsPatrol = true;
				MonsterMoveEventIndex = 0;
				ActivateHitCollision();
				ChangeActionType(MonsterActionType::MOVE);
			}
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

			GetWorld()->GetTimerManager().SetTimer(HeadTimer, FTimerDelegate::CreateLambda([=]()
				{
					GetWorld()->GetTimerManager().SetTimer(EndTimer, FTimerDelegate::CreateLambda([=]()
						{
							GetWorld()->GetTimerManager().ClearTimer(HeadTimer);
						}), RotateTime, false);

					HeadAnimInstance->Rot += RotVal + FRotator(AddVal.Pitch, AddVal.Yaw, AddVal.Roll);
					
				}), RotateTickTime, true);

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

			GetWorldTimerManager().ClearTimer(HeadTimer);
			GetWorldTimerManager().ClearTimer(EndTimer);

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
