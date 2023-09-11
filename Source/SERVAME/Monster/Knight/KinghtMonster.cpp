
#include "KinghtMonster.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RandomStream.h"
#include "KnightAttackTriggerComp.h"
#include "..\..\Manager\CombatManager.h"
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
			if (TracePlayer)
			{
				StateType = AnimTypeToStateType[MonsterAnimationType::IDLE];
				MonsterMoveEventIndex = 1;
				KnightAnimInstance->BlendSpeed = WalkBlend;
				KnightAnimInstance->Montage_Stop(0.25f, MontageMap[AnimationType]);

				ChangeActionType(MonsterActionType::MOVE);
				return;
			}

			KnightAnimInstance->BlendSpeed = IdleBlend;

			if (AttackAnimationType != MonsterAnimationType::NONE)
				StartAttackTrigger(AttackAnimationType);
		});
	NotifyBeginEndEventMap[MonsterAnimationType::IDLE].Add(false, [&]()
		{	
			if (TracePlayer)
			{
				StateType = AnimTypeToStateType[MonsterAnimationType::IDLE];
				MonsterMoveEventIndex = 1;
				KnightAnimInstance->BlendSpeed = WalkBlend;
				KnightAnimInstance->Montage_Stop(0.25f, MontageMap[AnimationType]);
				
				ChangeActionType(MonsterActionType::MOVE);
				return;
			}

			KnightAnimInstance->BlendSpeed = IdleBlend;

			if (AttackAnimationType != MonsterAnimationType::NONE)
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
			IsPatrol = true;
			GetCharacterMovement()->MaxWalkSpeed = MonsterDataStruct.CharacterOriginSpeed;
			KnightAnimInstance->BlendSpeed = WalkBlend;
			MonsterController->Patrol(PatrolActorArr[PatrolIndexCount]->GetActorLocation(), PatrolActorArr.Num());
		});
	MonsterMoveMap.Add(3, [&]()
		{
			if (CircleWalkEnd == false)
				MonsterController->MoveWhenArrived(CirclePoints[CircleIndexCount]);
		});

	MontageEndEventMap.Add(MonsterAnimationType::IDLE, [&]()
		{
			if (PlayerCharacter && !TracePlayer)
			{
				StartAttackTrigger(AttackAnimationType);
			}
			else if (TracePlayer)
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

	MontageEndEventMap.Add(MonsterAnimationType::ATTACK1, [&]()
		{
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

	MonsterTickEventMap.Add(MonsterActionType::MOVE, [&]()
		{
			StateType = AnimTypeToStateType[MonsterAnimationType::IDLE];

			if (CurrentDistance >= RunableDistance && !IsPatrol)
			{
				ChangeActionType(MonsterActionType::RUN);
			}
			else
			{
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
			MonsterDataStruct.RunSpeed = FMath::Lerp(120.f, 240.f, (KnightAnimInstance->BlendSpeed - 300.f) / 300.f);

			//KnightAnimInstance->Montage_Stop(0.25f, MontageMap[AnimationType]);
		});

	MonsterTickEventMap.Add(MonsterActionType::HIT, [&]()
		{
		});

	SetActionByRandomMap.Add(MonsterAnimationType::ATTACK1, [&](float percent)
		{
			ChangeActionType(MonsterActionType::ATTACK);
			ChangeMontageAnimation(MonsterAnimationType::ATTACK1);
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
	}
}

void AKinghtMonster::Tick(float DeltaTime)
{
	if (Spawning)
		return;

	Super::Tick(DeltaTime);

	if (IsInterpStart)
		InterpMove();

	if (MonsterController->FindPlayer)
		IsPatrol = false;

	if (MonsterController->FindPlayer && CurrentDistance > AttackRange && ActionType != MonsterActionType::ATTACK)
		TracePlayer = true;

	if (ActionType == MonsterActionType::RUN && CalcedDist != RunBlend && !IsPatrol)
	{
		if (WalkToRunBlend)
		{
			InterpolationTime += DeltaTime;
			CalcedDist = FMath::Lerp(WalkBlend, RunBlend, InterpolationTime / InterpolationDuration);
		}
		else if (CurrentDistance >= AccelerationDist)
		{
			InterpolationTime += DeltaTime;
			CalcedDist = FMath::Lerp(IdleBlend, WalkBlend, InterpolationTime / InterpolationDuration);
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
		OpactiyDeltaTime += 0.01;
		SkeletalMeshComp->SetScalarParameterValueOnMaterials("Dither", MeshOpacity -= OpactiyDeltaTime);
		KnightHeadMesh->SetScalarParameterValueOnMaterials("Dither", MeshOpacity -= OpactiyDeltaTime);
	}

	SearchPlayer();
}

void AKinghtMonster::RespawnCharacter()
{
	Super::RespawnCharacter();

	TracePlayer = false;
	MonsterController->FindPlayer = false;
	IsPatrol = true;
	MonsterMoveEventIndex = 0;
	ChangeActionType(MonsterActionType::MOVE);

	WeaponOpacity = 0.171653f;
	MeshOpacity = 0.171653f;

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
/*	GetWorld()->GetTimerManager().SetTimer(KnockBackTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			IsKnockBack = false;
			GetWorld()->GetTimerManager().ClearTimer(KnockBackTimerHandle);
		}), KnockBackTime, false);

	GetWorld()->GetTimerManager().SetTimer(KnockBackDelayTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			ActivateAttackTrigger();
			MontageEndEventMap[MonsterAnimationType::HIT]();
			GetWorld()->GetTimerManager().ClearTimer(KnockBackDelayTimerHandle);
		}), KnockBackDelayTime, false);

	StopAnimMontage(MontageMap[AnimationType]); 
	DeactivateAttackTrigger();
	IsKnockBack = true;*/ 
}

void AKinghtMonster::KnockBackEmd() 
{ 
	//IsKnockBack = false; 
}

void AKinghtMonster::SpawnBegin()
{
	Spawning = true;
	StateType = MonsterStateType::CANTACT;
	HitCollision->Deactivate();
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKinghtMonster::SpawnEnd()
{
	Spawning = false;
	StateType = MonsterStateType::NONE;
	HitCollision->Activate();
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AKinghtMonster::OnHitCancle()
{
	CanCancle = true;
}

void AKinghtMonster::OffHitCancle()
{
	CanCancle = false;
}

void AKinghtMonster::Stun()
{
	KnightAnimInstance->StopMontage(MontageMap[AnimationType]);
	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	ChangeMontageAnimation(MonsterAnimationType::DEAD);
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
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	}
}

void AKinghtMonster::OnKnightTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AttackAnimationType = MonsterAnimationType::NONE;
}

void AKinghtMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	TracePlayer = false;
	if (StateType == MonsterStateType::CANTACT || IsKnockBack == true)
		return;
	AttackAnimationType = AttackAnimType;
	if (ActionType != MonsterActionType::ATTACK)
	{
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

void AKinghtMonster::Rotate()
{
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP
		|| AnimationType == MonsterAnimationType::EXECUTION)
		return;
	SetActorRotation(FMath::Lerp(GetActorRotation(), YawRotation, MonsterDataStruct.RotateSpeed * fDeltaTime));
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
		HitType = MonsterAnimationType::HIT;
	else if (ForwardSpeed < 0)
		HitType = MonsterAnimationType::BACKHIT;
}

float AKinghtMonster::Die(float Dm)
{
	if (MonsterDataStruct.CharacterHp <= 0)
	{
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

		GetWorld()->GetTimerManager().SetTimer(MonsterDeadTimer, FTimerDelegate::CreateLambda([=]()
			{
				MinusOpacity = true;
			}), 3.2f, false);

		return Dm;
	}

	return Dm;
}

float AKinghtMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Spawning)
		return DamageAmount;

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

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

void AKinghtMonster::PlayExecutionAnimation()
{
	IsStun = false;
	CanExecution = false;
	ChangeMontageAnimation(MonsterAnimationType::EXECUTION);
}
