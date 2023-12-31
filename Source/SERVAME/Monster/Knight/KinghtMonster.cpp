#include "KinghtMonster.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RandomStream.h"
#include "KnightAttackTriggerComp.h"
#include "..\..\Manager\CombatManager.h"
#include "..\..\Manager\SoundManager.h"
#include "..\..\ObjectPool\EffectObjectInPool.h"
#include "GameFramework/CharacterMovementComponent.h"

AKinghtMonster::AKinghtMonster()
{
	AttackTrigger = CreateDefaultSubobject<UKnightAttackTriggerComp>(TEXT("AttackTriggerCollision"));
	AttackTrigger->SetupAttachment(GetMesh());

	ParryingCollision1->SetupAttachment(GetMesh(), FName("B_WEAPON"));
	SwordTrailComp->SetupAttachment(GetMesh(), FName("B_WEAPON"));
	WeaponCollision->SetupAttachment(GetMesh(), FName("B_WEAPON"));
	WeaponOverlapStaticMeshCollision->SetupAttachment(GetMesh(), FName("B_WEAPON"));
	WeaponOverlapStaticMeshCollision->SetCollisionProfileName("Weapon");

	KnightHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KnightHeadMesh"));
	KnightHeadMesh->SetupAttachment(GetMesh(), FName("Bip001-Head"));

	NotifyBeginEndEventMap.Add(MonsterAnimationType::IDLE, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::IDLE].Add(true, [&]()
		{
			if (TracePlayer && MonsterController->FindPlayer)
			{
				StateType = AnimTypeToStateType[MonsterAnimationType::IDLE];
				MonsterMoveEventIndex = 1;
				KnightAnimInstance->BlendSpeed = WalkBlend;
				KnightAnimInstance->Montage_Stop(0.25f, MontageMap[AnimationType]);

				ChangeActionType(MonsterActionType::MOVE);
				return;
			}

			KnightAnimInstance->BlendSpeed = IdleBlend;

			if (AttackAnimationType != MonsterAnimationType::NONE && MonsterController->FindPlayer)
				StartAttackTrigger(AttackAnimationType);
		});
	NotifyBeginEndEventMap[MonsterAnimationType::IDLE].Add(false, [&]()
		{	
			if (CurrentDistance < AttackRange)
			{
				StartAttackTrigger(AttackAnimationType);
			}

			if (TracePlayer && MonsterController->FindPlayer)
			{
				StateType = AnimTypeToStateType[MonsterAnimationType::IDLE];
				MonsterMoveEventIndex = 1;
				KnightAnimInstance->BlendSpeed = WalkBlend;
				KnightAnimInstance->Montage_Stop(0.25f, MontageMap[AnimationType]);
				
				ChangeActionType(MonsterActionType::MOVE);
				return;
			}

			KnightAnimInstance->BlendSpeed = IdleBlend;

			if (AttackAnimationType != MonsterAnimationType::NONE && MonsterController->FindPlayer)
				StartAttackTrigger(AttackAnimationType);
		});

	TargetDetectEventMap.Add(MonsterAttackType::MELEE, [&]()
		{
			ChangeActionType(MonsterActionType::MOVE);
			KnightAnimInstance->BlendSpeed = IdleBlend;
		});
	TargetDetectEventMap.Add(MonsterAttackType::RANGE, [&]()
		{
			ChangeActionType(MonsterActionType::MOVE);
			KnightAnimInstance->BlendSpeed = IdleBlend;
		});

	MonsterMoveMap.Add(0, [&]()
		{
			if (IsSpawn)
				return;

			if (PatrolActorArr.IsEmpty())
			{
				//isReturnBlend = true;
				//MonsterMoveEventIndex = 1;
				//ChangeActionType(MonsterActionType::NONE);
				IsPatrol = false;
				KnightAnimInstance->BlendSpeed = WalkBlend;
				MonsterController->MoveToStartLoc(SpawnLocation);
				return;
			}

			IsPatrol = true;
			GetCharacterMovement()->MaxWalkSpeed = MonsterDataStruct.CharacterOriginSpeed;
			KnightAnimInstance->BlendSpeed = WalkBlend;
			if (PatrolActorArr[PatrolIndexCount] != nullptr)
				MonsterController->Patrol(PatrolActorArr[PatrolIndexCount]->GetActorLocation(), PatrolActorArr.Num());
		});
	MonsterMoveMap.Add(3, [&]()
		{
			if (CircleWalkEnd == false)
				MonsterController->MoveWhenArrived(CirclePoints[CircleIndexCount]);
		});

	MontageEndEventMap.Add(MonsterAnimationType::IDLE, [&]()
		{
            /*if (PlayerCharacter && !TracePlayer)
			{
				StartAttackTrigger(AttackAnimationType);
			}
			else*/if (TracePlayer)
			{
				MonsterMoveEventIndex = 1;
				KnightAnimInstance->BlendSpeed = WalkBlend;
				ChangeActionType(MonsterActionType::MOVE);
			}
			else
			{
				ChangeMontageAnimation(MonsterAnimationType::IDLE);
			}
		});

	MontageEndEventMap.Add(MonsterAnimationType::ELITEKNIGHT_STAND_IDLE, [&]()
		{
			ChangeMontageAnimation(MonsterAnimationType::ELITEKNIGHT_STAND_IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::ELITEKNIGHT_START, [&]()
		{
			MonsterController->FindPlayer = true;
			TracePlayer = true;

			MonsterMoveEventIndex = 1;
			KnightAnimInstance->BlendSpeed = WalkBlend;
			ChangeActionType(MonsterActionType::MOVE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::EXECUTION, [&]()
		{
			AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			WalkToRunBlend = false;
			OnHitCancle();

			if (TracePlayer)
			{
				MonsterMoveEventIndex = 1;
				KnightAnimInstance->BlendSpeed = WalkBlend;
				ChangeActionType(MonsterActionType::MOVE);
			}
			else
			{
				ChangeActionType(MonsterActionType::NONE);
				ChangeMontageAnimation(MonsterAnimationType::IDLE);
			}
		});

	MontageEndEventMap.Add(MonsterAnimationType::DEAD, [&]()
		{
			KnightAnimInstance->PauseAnimation(MontageMap[AnimationType]);
		});

	MontageEndEventMap.Add(MonsterAnimationType::GROGGY_DEAD, [&]()
		{
			KnightAnimInstance->PauseAnimation(MontageMap[AnimationType]);
		});
	
	MontageEndEventMap.Add(MonsterAnimationType::ATTACK1, [&]()
		{
			if (IsPatrol)
				return;

			AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			WalkToRunBlend = false;
			CanRotate = true;
			OnHitCancle();

			if (!MonsterController->FindPlayer)
			{
				ChangeActionType(MonsterActionType::MOVE);
				return;
			}

			if (TracePlayer)
			{
				MonsterMoveEventIndex = 1;
				KnightAnimInstance->BlendSpeed = WalkBlend;
				ChangeActionType(MonsterActionType::MOVE);
			}
			else
			{
				ChangeActionType(MonsterActionType::NONE);
				ChangeMontageAnimation(MonsterAnimationType::IDLE);
			}
		});

	MontageEndEventMap.Add(MonsterAnimationType::POWERATTACK1, MontageEndEventMap[MonsterAnimationType::ATTACK1]);

	MontageEndEventMap.Add(MonsterAnimationType::SPRINTATTACK, [&]()
		{
			if (IsPatrol)
				return;

			AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			WalkToRunBlend = false;
			OnHitCancle();

			if (TracePlayer)
			{
				MonsterMoveEventIndex = 1;
				KnightAnimInstance->BlendSpeed = WalkBlend;
				ChangeActionType(MonsterActionType::MOVE);
			}
			else
			{
				ChangeActionType(MonsterActionType::NONE);
				ChangeMontageAnimation(MonsterAnimationType::IDLE);
			}
		});

	MontageEndEventMap.Add(MonsterAnimationType::HIT, [&]()
		{
			OnHitCancle();

			if (TracePlayer)
			{
				MonsterMoveEventIndex = 1;
				KnightAnimInstance->BlendSpeed = WalkBlend;
				ChangeActionType(MonsterActionType::MOVE);
			}
			else
			{
				ChangeActionType(MonsterActionType::NONE);
				ChangeMontageAnimation(MonsterAnimationType::IDLE);
			}
		});

	MontageEndEventMap.Add(MonsterAnimationType::BACKHIT, [&]()
		{
			if (TracePlayer)
			{
				MonsterMoveEventIndex = 1;
				KnightAnimInstance->BlendSpeed = WalkBlend;
				ChangeActionType(MonsterActionType::MOVE);
			}
			else
			{
				ChangeActionType(MonsterActionType::NONE);
				ChangeMontageAnimation(MonsterAnimationType::IDLE);
			}
		});

	MontageEndEventMap.Add(MonsterAnimationType::GROGGY_START, [&]()
		{
			if (MyMonsterType != MonsterType::ELITEKNIGHT)
			{
				CanRotate = false;
				ChangeMontageAnimation(MonsterAnimationType::GROGGY_LOOP);
			}
			else
			{
				CanRotate = true;
				AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				WalkToRunBlend = false;
				OnHitCancle();

				if (TracePlayer)
				{
					MonsterMoveEventIndex = 1;
					KnightAnimInstance->BlendSpeed = WalkBlend;
					ChangeActionType(MonsterActionType::MOVE);
				}
				else
				{
					ChangeActionType(MonsterActionType::NONE);
					ChangeMontageAnimation(MonsterAnimationType::IDLE);
				}
			}
		});

	MontageEndEventMap.Add(MonsterAnimationType::GROGGY_LOOP, [&]()
		{
			ChangeMontageAnimation(MonsterAnimationType::GROGGY_LOOP);
		});

	MonsterTickEventMap.Add(MonsterActionType::MOVE, [&]()
		{
			StateType = AnimTypeToStateType[MonsterAnimationType::IDLE];

			if (CurrentDistance >= RunableDistance && !IsPatrol && MonsterController->FindPlayer)
			{
				ChangeActionType(MonsterActionType::RUN);
			}
			else
			{
				if(TracePlayer)
				{
					if (!IsMoveStart)
						MinWalkTime = GetRandNum(1, 2);

					IsMoveStart = true;
				}
				else
					IsMoveStart = false;

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

	MonsterTickEventMap.Add(MonsterActionType::RUN, [&]()
		{
			if (!MonsterController->FindPlayer)
				return;

			if (IsPatrol)
			{
				ChangeActionType(MonsterActionType::MOVE);
				KnightAnimInstance->BlendSpeed = WalkBlend;
				return;
			}

			RotateMap[PlayerCharacter != nullptr]();
			MonsterMoveMap[MonsterMoveEventIndex]();

			auto speed = FMath::Clamp(CalcedDist, 0.f, 600.f);

			if (speed > Temp)
				Temp = speed;

			KnightAnimInstance->BlendSpeed = Temp;
			MonsterDataStruct.RunSpeed = FMath::Lerp(120.f, 600.f, (KnightAnimInstance->BlendSpeed - 300.f) / 300.f);

			//KnightAnimInstance->Montage_Stop(0.25f, MontageMap[AnimationType]);
		});

	MonsterTickEventMap.Add(MonsterActionType::SPRINT, [&]()
		{
			AttackTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			RotateMap[PlayerCharacter != nullptr]();
			MonsterMoveMap[MonsterMoveEventIndex]();

			auto speed = FMath::Clamp(CalcedDist, 300.f, 600.f);


			if (speed > Temp)
				Temp = speed;

			KnightAnimInstance->BlendSpeed = Temp;
			MonsterDataStruct.RunSpeed = FMath::Lerp(120.f, 600.f, (KnightAnimInstance->BlendSpeed - 300.f) / 300.f);

			//KnightAnimInstance->Montage_Stop(0.25f, MontageMap[AnimationType]);
		});

	MonsterTickEventMap.Add(MonsterActionType::HIT, [&]()
		{
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

	SetActionByRandomMap.Add(MonsterAnimationType::DASHATTACK1, [&](float percent)
		{
				MonsterMoveEventIndex = 1;
				ChangeActionType(MonsterActionType::MOVE);
				KnightAnimInstance->BlendSpeed = WalkBlend;
		});
}

void AKinghtMonster::BeginPlay()
{
	Super::BeginPlay(); 

	if (MyMonsterType != MonsterType::PERSISTENTKNIGHT && MyMonsterType != MonsterType::DEADBODYOFKNIGHT)
		Reviving = false;

	if (IsSpawnKnight)
	{
		SetActive(false);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
		MonsterController->CanPerception = false;
	}
	else
	{
		MonsterController->CanPerception = true;
	}

	DeactivateHpBar();

	if(IsPatrol)
	{
		TracePlayer = false;
		MonsterController->FindPlayer = false;
		IsPatrol = true;
		MonsterMoveEventIndex = 0;
		ChangeActionType(MonsterActionType::MOVE);
	}


	if (IsSpawn)
	{
	}

	SetActive(true);

	PlayerCharacter = nullptr;

	KnightAnimInstance = Cast<UKnightAnimInstance>(GetMesh()->GetAnimInstance());
	
	TargetDetectionCollison->OnComponentBeginOverlap.AddDynamic(this, &AKinghtMonster::OnKnightTargetDetectionBeginOverlap);
	TargetDetectionCollison->OnComponentEndOverlap.AddDynamic(this, &AKinghtMonster::OnKnightTargetDetectionEndOverlap);

	if (KnightAnimInstance != nullptr)
	{
		KnightAnimInstance->InterpStart.AddUObject(this, &AKinghtMonster::InterpStart);
		KnightAnimInstance->InterpEnd.AddUObject(this, &AKinghtMonster::InterpEnd);
		KnightAnimInstance->KnockBackStart.AddUObject(this, &AKinghtMonster::KnockBackStart);
		KnightAnimInstance->KnockBackEnd.AddUObject(this, &AKinghtMonster::KnockBackEmd);
		KnightAnimInstance->SpawningBegin.AddUObject(this, &AKinghtMonster::SpawnBegin);
		KnightAnimInstance->SpawningEnd.AddUObject(this, &AKinghtMonster::SpawnEnd);
		KnightAnimInstance->CanHitCancle.AddUObject(this, &AKinghtMonster::OnHitCancle);
		KnightAnimInstance->CantHitCancle.AddUObject(this, &AKinghtMonster::OffHitCancle);
		KnightAnimInstance->CanRotate.AddUObject(this, &AKinghtMonster::OnRotate);
		KnightAnimInstance->CantRotate.AddUObject(this, &AKinghtMonster::OffRotate);
	}
}

void AKinghtMonster::Tick(float DeltaTime)
{
	if (Spawning)
	{
		OpactiyDeltaTime += 0.0001;
		SkeletalMeshComp->SetScalarParameterValueOnMaterials("Dither", MeshOpacity += OpactiyDeltaTime);
		KnightHeadMesh->SetScalarParameterValueOnMaterials("Dither", MeshOpacity += OpactiyDeltaTime);
		return;
	}

	Super::Tick(DeltaTime);

	if (IsInterpStart)
		InterpMove();

	if (MonsterController->FindPlayer)
		IsPatrol = false;

	if (MonsterController->FindPlayer && CurrentDistance > AttackRange && ActionType != MonsterActionType::ATTACK)
		TracePlayer = true;

	if ((ActionType == MonsterActionType::RUN || ActionType == MonsterActionType::SPRINT)
		&& CalcedDist != RunBlend && !IsPatrol)
	{
		if (WalkToRunBlend)
		{
			InterpolationTime += DeltaTime;
			CalcedDist = FMath::Lerp(WalkBlend, RunBlend, InterpolationTime / InterpolationDuration);
		}
		else if (CurrentDistance >= AccelerationDist)
		{
			InterpolationTime += DeltaTime;
			CalcedDist = FMath::Lerp(IdleBlend, RunBlend, InterpolationTime / InterpolationDuration);
		}
		else
		{
			InterpolationTime = 0;
			CalcedDist = RunBlend;
		}

		GetCharacterMovement()->MaxWalkSpeed = MonsterDataStruct.RunSpeed;
	}
	else
	{
		InterpolationTime = 0;
	}

	if (MinusOpacity)
	{
		OpactiyDeltaTime += 0.005;
		SkeletalMeshComp->SetScalarParameterValueOnMaterials("Dither", MeshOpacity -= OpactiyDeltaTime);
		KnightHeadMesh->SetScalarParameterValueOnMaterials("Dither", MeshOpacity -= OpactiyDeltaTime);
	}

	if (isReturnBlend)
		ReturnBlendFunc(DeltaTime);

	SearchPlayer();
}

void AKinghtMonster::ReturnBlendFunc(float delta)
{
	ReturnInterpTime += delta;
	auto Val = FMath::Clamp(CalcedDist, IdleBlend, RunBlend);

	KnightAnimInstance->BlendSpeed = FMath::Clamp(FMath::Lerp(Val, IdleBlend, ReturnInterpTime / InterpolationDuration), IdleBlend, RunBlend);
	MonsterDataStruct.RunSpeed = FMath::Clamp(FMath::Lerp(GetCharacterMovement()->MaxWalkSpeed, 0.f, ReturnInterpTime / InterpolationDuration), IdleBlend, RunBlend);

	//UE_LOG(LogTemp, Warning, TEXT("%f"), ReturnInterpTime);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), FMath::Lerp(Val, IdleBlend, ReturnInterpTime / InterpolationDuration));

	GetCharacterMovement()->MaxWalkSpeed = MonsterDataStruct.RunSpeed;

	if (KnightAnimInstance->BlendSpeed <= IdleBlend)
	{
		MonsterController->StopMovement();
		ChangeMontageAnimation(MonsterAnimationType::IDLE);
		isReturnBlend = false;
		ReturnInterpTime = 0.f;
	}
}

void AKinghtMonster::IdleToWalkBlendFunc(float delta)
{
	IdleToWalkInterpTime += delta;
	auto Val = FMath::Clamp(CalcedDist, IdleBlend, WalkBlend);

	KnightAnimInstance->BlendSpeed = FMath::Clamp(FMath::Lerp(Val, IdleBlend, IdleToWalkInterpTime / InterpolationDuration), IdleBlend, WalkBlend);
	auto Speed = FMath::Clamp(FMath::Lerp(GetCharacterMovement()->MaxWalkSpeed, 120.f, IdleToWalkInterpTime / InterpolationDuration), IdleBlend, WalkBlend);

	//UE_LOG(LogTemp, Warning, TEXT("%f"), IdleToWalkInterpTime);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), FMath::Lerp(Val, IdleBlend, IdleToWalkInterpTime / InterpolationDuration));

	GetCharacterMovement()->MaxWalkSpeed = Speed;

	if (KnightAnimInstance->BlendSpeed >= WalkBlend)
	{
		IdleToWalkInterpTime = 0.f;
	}
}

void AKinghtMonster::RespawnCharacter()
{
	if (IsDie)return;
	if (IsSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("spawned knight delete"));
		//소환된 기사 삭제
		auto index = PlayerCharacter->HitMonsterInfoArray.Find(this);
		PlayerCharacter->HitMonsterInfoArray.RemoveAtSwap(index);
		SetActive(false);		
		GetWorld()->DestroyActor(this);
		return;
	}

	Super::RespawnCharacter();

	UE_LOG(LogTemp, Warning, TEXT("knight respawn"));

	MonsterController->FindPlayer = false;
	LockOnComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	KnightAnimInstance->ResumeMontage(MontageMap[AnimationType]);
	GetWorld()->GetTimerManager().ClearTimer(MonsterDeadTimer);
	InterpolationTime = 0.f;

	//TODO : 일반 기사, 끈질긴 기사 패트롤
	if (MyMonsterType == MonsterType::KNIGHT || MyMonsterType == MonsterType::PERSISTENTKNIGHT)
	{
		TracePlayer = false;
		IsPatrol = true;
		MonsterMoveEventIndex = 0;
		ChangeActionType(MonsterActionType::MOVE);
	}		
	else if (MyMonsterType == MonsterType::DEADBODYOFKNIGHT)
	{
		Imotal = true;
		
		AnimationType = MonsterAnimationType::STARTDEAD;
		ChangeActionType(MonsterActionType::NONE);
		ChangeMontageAnimation(MonsterAnimationType::STARTDEAD);
	}
	else if (MyMonsterType == MonsterType::ELITEKNIGHT)
	{
		TracePlayer = false;
		CalcedDist = IdleBlend;
		ChangeActionType(MonsterActionType::NONE);
		if (!IsBoss)
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		else
			ChangeMontageAnimation(MonsterAnimationType::ELITEKNIGHT_STAND_IDLE);
	}

	WeaponOpacity = 1.0f;
	MeshOpacity = 1.0f;

	GetCapsuleComponent()->SetCollisionProfileName("AIPhysics");
	SkeletalMeshComp->SetScalarParameterValueOnMaterials("Dither", MeshOpacity);
	KnightHeadMesh->SetScalarParameterValueOnMaterials("Dither", WeaponOpacity);

	MinusOpacity = false;
	OpactiyDeltaTime = 0.f;

	ActivateHitCollision();
	MonsterDataStruct.CharacterHp = MonsterDataStruct.CharacterMaxHp;
	MonsterHPWidget->SetHP(1.0f);
}

void AKinghtMonster::ResumeMontage()
{
	if (MontageMap.Contains(AnimationType))
		KnightAnimInstance->ResumeMontage(MontageMap[AnimationType]);
}

void AKinghtMonster::InterpStart() { IsInterpStart = true; }

void AKinghtMonster::InterpEnd() { IsInterpStart = false; }

void AKinghtMonster::KnockBackStart()
{
	GetWorld()->GetTimerManager().SetTimer(KnockBackTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			IsKnockBack = false;
			GetWorld()->GetTimerManager().ClearTimer(KnockBackTimerHandle);
		}), KnockBackTime, false);

	GetWorld()->GetTimerManager().SetTimer(KnockBackDelayTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			ActivateAttackTrigger();
			GetWorld()->GetTimerManager().ClearTimer(KnockBackDelayTimerHandle);
		}), KnockBackDelayTime, false);

	DeactivateAttackTrigger();
	IsKnockBack = true; 
}

void AKinghtMonster::KnockBackEmd() 
{ 
	IsKnockBack = false; 
}

void AKinghtMonster::SpawnBegin()
{
	CanCancle = false;
	Spawning = true;
	StateType = MonsterStateType::CANTACT;
	HitCollision->Deactivate();
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKinghtMonster::SpawnEnd()
{
	Imotal = false;
	CanCancle = true;
	Spawning = false;
	StateType = MonsterStateType::NONE;
	HitCollision->Activate();
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AKinghtMonster::OnHitCancle()
{
	if (MyMonsterType == MonsterType::ELITEKNIGHT)
		CanCancle = false;
	else
		CanCancle = true;
}

void AKinghtMonster::OffHitCancle()
{
	CanCancle = false;
}

void AKinghtMonster::OnRotate()
{
	CanRotate = true;
}

void AKinghtMonster::OffRotate()
{
	CanRotate = false;
}

void AKinghtMonster::Stun()
{
	if (Reviving)
		return;

	if (MyMonsterType != MonsterType::ELITEKNIGHT)
		CanExecution = true;

	CanRotate = false;
	IsStun = true;
	KnightAnimInstance->StopMontage(MontageMap[AnimationType]);
	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	ChangeMontageAnimation(MonsterAnimationType::GROGGY_START);
}

void AKinghtMonster::ParryingStun()
{
	//if (MyMonsterType != MonsterType::ELITEKNIGHT)
	//	CanExecution = true;
	KnightAnimInstance->StopMontage(MontageMap[AnimationType]);
	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	ChangeMontageAnimation(MonsterAnimationType::PARRYING);
}

void AKinghtMonster::PlayExecutionAnimation()
{
	if (MyMonsterType != MonsterType::ELITEKNIGHT)
		MonsterDataStruct.CharacterHp = 1;
	IsStun = false;
	CanExecution = false;
	ChangeMontageAnimation(MonsterAnimationType::EXECUTION);
}

void AKinghtMonster::ChangeMontageAnimation(MonsterAnimationType type)
{
	if (KnightAnimInstance == nullptr)
	{
		KnightAnimInstance = Cast<UKnightAnimInstance>(GetMesh()->GetAnimInstance());
	}
	else
	{
		if (MontageMap.Contains(AnimationType))
			KnightAnimInstance->StopMontage(MontageMap[AnimationType]);
		AnimationType = type;

		if (AnimTypeToStateType.Contains(type))
			StateType = AnimTypeToStateType[type];

		if (MontageMap.Contains(type))
			KnightAnimInstance->PlayMontage(MontageMap[type]);
	}
}

void AKinghtMonster::InterpMove()
{
	auto NewLoc = FMath::Lerp(GetActorLocation(), PlayerCharacter->GetActorLocation(), 0.1f);
	SetActorLocation(NewLoc);
}

void AKinghtMonster::MonsterHitStop()
{
	if (MontageMap.Contains(AnimationType))
		KnightAnimInstance->PauseAnimation(MontageMap[AnimationType]);
}
void AKinghtMonster::ActivateAttackTrigger()
{
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AKinghtMonster::DeactivateAttackTrigger()
{
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKinghtMonster::OnKnightTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO : 
	//소환되자 마자 플레이어 추격 하도록 변경?
	//ActivateHpBar();

	if (ActionType == MonsterActionType::DEAD)
		return;

	if (MyMonsterType == MonsterType::ELITEKNIGHT)
	{
		ASoundManager::GetInstance().PauseBGMByLerp(BGMType::TITLEINTRO);
	}

	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		Super::PlayerCharacter = PlayerCharacter;
	}
}

void AKinghtMonster::OnKnightTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AttackAnimationType = MonsterAnimationType::NONE;
}

void AKinghtMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	TracePlayer = false;
	CanRotate = true;
	if (StateType == MonsterStateType::CANTACT || IsKnockBack == true)
		return;
	AttackAnimationType = AttackAnimType;
	if (ActionType != MonsterActionType::ATTACK)
	{
		IsMoveStart = false;
		SprintDeltaTime = 0;

		MonsterController->StopMovement();

		if (MontageMap.Contains(AnimationType))
			KnightAnimInstance->StopMontage(MontageMap[AnimationType]);

		float RandomValue = FMath::RandRange(0, 100) * 0.01f;
		if (SetActionByRandomMap.Contains(AttackAnimType))
		{
			MonsterMoveEventIndex = 1;
			SetActionByRandomMap[AttackAnimType](RandomValue);
		}
	}
}

void AKinghtMonster::EndAttackTrigger(MonsterAnimationType AttackAnimType)
{
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP)
		return;
	TracePlayer = true;
}

void AKinghtMonster::DrawCircle(FVector Center)
{
	CirclePoints.SetNum(NumSegments);

	const float AngleBetweenSegments = 2 * PI / NumSegments;

	// 원을 구성하는 점들 계산
	int ClosestPointIndex = 0;
	float MinDistance = FLT_MAX;

	for (int i = 1; i < NumSegments; ++i)
	{
		float Angle = i * AngleBetweenSegments;
		float X = Center.X + Radius * FMath::Cos(Angle);
		float Y = Center.Y + Radius * FMath::Sin(Angle);
		FVector Point(X, Y, Center.Z);
		CirclePoints[i] = Point;

		float Distance = FVector::Dist(Point, GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestPointIndex = i;
		}
	}

	// ClosestPointIndex부터 시작하여 원의 점들 재배열
	TArray<FVector> TempArray;
	TempArray.Append(CirclePoints.GetData() + ClosestPointIndex, CirclePoints.Num() - ClosestPointIndex);
	TempArray.Append(CirclePoints.GetData() + 1, ClosestPointIndex);
	CirclePoints = TempArray;

	if (DrawDebugCircle)
	{
		// 점들을 이어서 원 그리기
		for (int32 i = 0; i < NumSegments; ++i)
		{
			FVector Start = CirclePoints[i];
			FVector End = CirclePoints[(i + 1) % NumSegments];

			if (i == 0)
				DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 5.0f);
			else
				DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 5.0f);
		}
	}
}

void AKinghtMonster::SearchPlayer()
{
	if (PlayerCharacter == nullptr)
		return;

	auto TargetLoc = PlayerCharacter->GetActorLocation() - GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();
	float ForwardSpeed = FVector::DotProduct(TargetLoc, Forward);
	float RightSpeed = FVector::DotProduct(TargetLoc, Right);

	if (ForwardSpeed > 0)
	{
		if (PlayerCharacter->CurActionType == ActionType::POWERATTACK ||
			PlayerCharacter->CurActionType == ActionType::SKILL)
			HitType = MonsterAnimationType::SUPER_HIT;

		HitType = MonsterAnimationType::HIT;
	}
	else if (ForwardSpeed < 0)
	{
		HitType = MonsterAnimationType::BACKHIT;
	}
}

float AKinghtMonster::Die(float Dm)
{
	if (PlayerCharacter->IsLockOn)
	{
		PlayerCharacter->LockOn();
	}

	LockOnComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AObjectPool& objectpool = AObjectPool::GetInstance();
	for (int32 i = 0; i < MonsterDataStruct.DropSoulCount; i++)
	{
		float x = FMath::RandRange(-300.0f, 300.0f);
		float y = FMath::RandRange(-300.0f, 300.0f);
		float z = FMath::RandRange(-300.0f, 300.0f);

		FVector location = GetActorLocation() + FVector(x * 0.1f, y * 0.1f, z * 0.1f);
		FRotator rotation = GetActorRotation() + FRotator(x, y, z);

		objectpool.SpawnObject(objectpool.ObjectArray[36].ObjClass, location, rotation);
	}
	
	PlayMonsterSoundInPool(EMonsterAudioType::KNIGHT_DIE);

	//if (IsSpawn)
	//{
	//	auto index = UCombatManager::GetInstance().HitMonsterInfoArray.Find(this);
	//	UCombatManager::GetInstance().HitMonsterInfoArray.RemoveAt(index);
	//}

	auto index = PlayerCharacter->HitMonsterInfoArray.Find(this);
	//UCombatManager::GetInstance().HitMonsterInfoArray.RemoveAtSwap(index);

	Imotal = true;
	IsDie = true;
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	DeactivateHpBar();
	DeactivateHitCollision();

	KnightAnimInstance->StopMontage(MontageMap[AnimationType]);

	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();

	ChangeActionType(MonsterActionType::DEAD);
	StateType = MonsterStateType::CANTACT;

	if (AnimationType == MonsterAnimationType::EXECUTION)
		ChangeMontageAnimation(MonsterAnimationType::GROGGY_DEAD);
	else
		ChangeMontageAnimation(MonsterAnimationType::DEAD);

	GetWorld()->GetTimerManager().SetTimer(MonsterDeadTimer, FTimerDelegate::CreateLambda([=]()
		{
			auto Pos = GetActorLocation();
			auto PoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[44].ObjClass,
				FVector(Pos.X, Pos.Y, Pos.Z - 50.f), FRotator::ZeroRotator);
			auto CastObj = Cast<AEffectObjectInPool>(PoolObj);
			CastObj->SetEffectType(SelectEffectType::KNIGHT_DEAD);
			CastObj->ActivateCurrentEffect();

			MinusOpacity = true;
		}), 4.f, false);

	return Dm;
}

float AKinghtMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Reviving)
		return 0.f;

	if (Imotal)
		return 0.f;

	if (Spawning)
		return 0.f;

	if (!Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser))
		return 0.0f;

	int value = FMath::RandRange(16, 17);
	AObjectPool& objectpool = AObjectPool::GetInstance();
	objectpool.SpawnObject(objectpool.ObjectArray[value].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
	HitStop();

	IsInterpStart = false;
	DeactivateHitCollision();

	if (AnimationType == MonsterAnimationType::EXECUTION)
		return 0.f;

	if (DamageAmount >= 30 && MonsterDataStruct.CharacterHp > 0)
	{
		if (CanCancle)
		{
			MonsterController->StopMovement();

			KnightAnimInstance->StopMontage(MontageMap[AnimationType]);
			if (MontageEndEventMap.Contains(AnimationType))
				MontageEndEventMap[AnimationType]();

			//TODO : 앞 뒤 방향에 따른 피격
			ChangeActionType(MonsterActionType::HIT);
			ChangeMontageAnimation(HitType);
		}
	}

	return DamageAmount;
}

void AKinghtMonster::IsNotifyActive(bool value)
{
	if (NotifyBeginEndEventMap.Contains(AnimationType))
	{
		NotifyBeginEndEventMap[AnimationType][value]();
	}
}

void AKinghtMonster::CheckMontageEndNotify()
{
	if (MontageEndEventMap.Contains(AnimationType))
	{
		MontageEndEventMap[AnimationType]();
	}
}

