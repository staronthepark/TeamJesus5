// Fill out your copyright notice in the Description page of Project Settings.


#include "NunMonster.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RandomStream.h"
#include "..\..\Manager\CombatManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NunAttackTriggerComp.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "NavigationSystem.h"
#include <Kismet/KismetMathLibrary.h>
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "..\..\ObjectPool\NunEffectObjInPool.h"
#include "..\..\NunDamageSphereTriggerComp.h"

int ANunMonster::CurrentNum = 0;

ANunMonster::ANunMonster()
{
	AttackTrigger = CreateDefaultSubobject<UNunAttackTriggerComp>(TEXT("AttackTriggerCollision"));
	AttackTrigger->SetupAttachment(GetMesh());

	ProjectileRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileRootComp"));
	ProjectileRootComp->SetupAttachment(RootComponent);

	CheckPerceptionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PerceptionCheckTrigger"));
	CheckPerceptionTrigger->SetupAttachment(GetMesh());
	CheckPerceptionTrigger->SetCollisionProfileName("DetectPlayer");

	Loc1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Loc1"));
	Loc1->SetupAttachment(ProjectileRootComp);
	Loc2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Loc2"));
	Loc2->SetupAttachment(ProjectileRootComp);
	Loc3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Loc3"));
	Loc3->SetupAttachment(ProjectileRootComp);
	Loc4 = CreateDefaultSubobject<UBoxComponent>(TEXT("Loc4"));
	Loc4->SetupAttachment(ProjectileRootComp);
	Loc5 = CreateDefaultSubobject<UBoxComponent>(TEXT("Loc5"));
	Loc5->SetupAttachment(ProjectileRootComp);
	Loc6 = CreateDefaultSubobject<UBoxComponent>(TEXT("Loc6"));
	Loc6->SetupAttachment(ProjectileRootComp);

	CrystalSpawnLoc = CreateDefaultSubobject<UBoxComponent>(TEXT("CrystalLoc"));
	CrystalSpawnLoc->SetupAttachment(ProjectileRootComp);

	AnimTypeToStateType.Add(MonsterAnimationType::HEAL1, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::HEAL2, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::SPAWNKNIGHT, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::SELFHEAL, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::WORSHIP, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::JUDEMENT, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::DARK, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::PRAY, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::CURSE, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::FRAGMENT, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::CRYSTAL, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::ILLUSION, MonsterStateType::BEFOREATTACK);

	MonsterMoveMap.Add(1, [&]()
		{
		});

	TargetDetectEventMap.Add(MonsterAttackType::MELEE, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});
	
	NotifyBeginEndEventMap.Add(MonsterAnimationType::IDLE, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::IDLE].Add(true, [&]()
		{
		});
	NotifyBeginEndEventMap[MonsterAnimationType::IDLE].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::HEAL1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::HEAL1].Add(true, [&]()
		{
			if (!MonsterController->FindPlayer)
				return;
			SpawnMagicCircle();
			SingleHeal();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::HEAL1].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::HEAL2, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::HEAL2].Add(true, [&]()
		{
			if (!MonsterController->FindPlayer)
				return;
			SpawnMagicCircle();
			MultiHeal();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::HEAL2].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::SPAWNKNIGHT, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::SPAWNKNIGHT].Add(true, [&]()
		{
			if (!MonsterController->FindPlayer)
				return;
			SpawnMagicCircle();
			SpawnKnight();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::SPAWNKNIGHT].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::WORSHIP, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::WORSHIP].Add(true, [&]()
		{
			if (!MonsterController->FindPlayer)
				return;
			SpawnMagicCircle();
			DotFloor();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::WORSHIP].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::JUDEMENT, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::JUDEMENT].Add(true, [&]()
		{
			if (!MonsterController->FindPlayer)
				return;
			SpawnMagicCircle();
			JudementAttack();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::JUDEMENT].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::CURSE, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::CURSE].Add(true, [&]()
		{
			if (!MonsterController->FindPlayer)
				return;
			SpawnMagicCircle();
			Curse();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::CURSE].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::SELFHEAL, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::SELFHEAL].Add(true, [&]()
		{
			if (!MonsterController->FindPlayer)
				return;
			SpawnMagicCircle();
			SelfHeal();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::SELFHEAL].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::DARK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::DARK].Add(true, [&]()
		{
			if (!MonsterController->FindPlayer)
				return;
			SpawnMagicCircle();
			DarkAttack();
		});

	NotifyBeginEndEventMap[MonsterAnimationType::DARK].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::PRAY, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::PRAY].Add(true, [&]()
		{
			if (!MonsterController->FindPlayer)
				return;
			SpawnMagicCircle();
			PrayAttack();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::PRAY].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::FRAGMENT, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::FRAGMENT].Add(true, [&]()
		{
			if (!MonsterController->FindPlayer)
				return;
			SpawnMagicCircle();
			FragmentsAttack();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::FRAGMENT].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::CRYSTAL, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::CRYSTAL].Add(true, [&]()
		{
			if (!MonsterController->FindPlayer)
				return;
			SpawnMagicCircle();
			CrystalAttack();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::CRYSTAL].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::ILLUSION, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::ILLUSION].Add(true, [&]()
		{
			if (!MonsterController->FindPlayer)
				return;
			SpawnMagicCircle();
			IllusionAttack();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::ILLUSION].Add(false, [&]()
		{
		});

	MontageEndEventMap.Add(MonsterAnimationType::IDLE, [&]()
		{
			if (PlayerCharacter && AnimationType != MonsterAnimationType::FRAGMENT)
				StartAttackTrigger(AttackAnimationType);
			else
				ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::HEAL1, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::HEAL2, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::SPAWNKNIGHT, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::SELFHEAL, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::JUDEMENT, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::WORSHIP, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::DARK, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::PRAY, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::FRAGMENT, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::CRYSTAL, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::ILLUSION, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::CURSE, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MonsterTickEventMap.Add(MonsterActionType::NONE, [&]()
		{
			//if (MonsterController->FindPlayer)
			//	RotateMap[PlayerCharacter != nullptr]();
		});

	MonsterTickEventMap.Add(MonsterActionType::ATTACK, [&]()
		{
			if (MonsterController->FindPlayer == false)
				MonsterController->FindPlayer = true;

			//RotateMap[true]();
		});

	MonsterTickEventMap.Add(MonsterActionType::POWERATTACK, [&]()
		{
			//RotateMap[true]();
		});

	MonsterTickEventMap.Add(MonsterActionType::MOVE, [&]()
		{
			if (MonsterController->FindPlayer == false)
			{
				MonsterController->StopMovement();
				ChangeActionType(MonsterActionType::MOVE);
			}
			else
			{
				//RotateMap[PlayerCharacter != nullptr]();
				MonsterMoveMap[MonsterMoveEventIndex]();
			}
		});

	SetActionByRandomMap.Add(MonsterAnimationType::DARK, [&](float percent)
		{
			if (percent <= Dark_Group_Percent_1)
			{
				//파편
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::FRAGMENT);
			}
			else if (percent > Dark_Group_Percent_1 && percent < Dark_Group_Percent_2)
			{
				//심판
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::CRYSTAL);
			}
			else
			{
				//저주
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::HEAL1);
				FogAttack();
			}
		});

	SetActionByRandomMap.Add(MonsterAnimationType::HEAL1, [&](float percent)
		{
			if (percent <= SingleHeal_Group_Percent_1)
			{
				//단일힐
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::HEAL1);
			}
			else if (percent > SingleHeal_Group_Percent_1 && percent < SingleHeal_Group_Percent_2)
			{
				//숭배
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::WORSHIP);
			}
			else
			{
				//심판
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::JUDEMENT);
			}
		});

	SetActionByRandomMap.Add(MonsterAnimationType::HEAL2, [&](float percent)
		{
			if (percent <= MultiHeal_Group_Percent_1)
			{
				//어둠
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::DARK);
			}
			else if (percent > MultiHeal_Group_Percent_1 && percent < MultiHeal_Group_Percent_2)
			{
				//광역힐
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::HEAL2);
			}
			else
			{
				//기도
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::PRAY);
			}
		});

	SetActionByRandomMap.Add(MonsterAnimationType::SPAWNKNIGHT, [&](float percent)
		{
			ChangeActionType(MonsterActionType::ATTACK);
			ChangeMontageAnimation(MonsterAnimationType::SPAWNKNIGHT);
		});
}

void ANunMonster::BeginPlay()
{
	Super::BeginPlay();

	if (MyMonsterType == MonsterType::NUN)
		DeactivateHpBar();

	SpawnLocArr.Push(Loc1);
	SpawnLocArr.Push(Loc2);
	SpawnLocArr.Push(Loc3);
	SpawnLocArr.Push(Loc4);
	SpawnLocArr.Push(Loc5);
	SpawnLocArr.Push(Loc6);

	NunAnimInstance = Cast<UNumAnimInstance>(GetMesh()->GetAnimInstance());
	WeaponOverlapStaticMeshCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponCollision->SetupAttachment(GetMesh(), FName("Nun_Weapon_Bone"));

	PlayerCharacter = nullptr;
	
	CheckPerceptionTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANunMonster::OnPerceptionTriggerBeginOverlap);
	TargetDetectionCollison->OnComponentBeginOverlap.AddDynamic(this, &ANunMonster::OnNunTargetDetectionBeginOverlap);
	TargetDetectionCollison->OnComponentEndOverlap.AddDynamic(this, &ANunMonster::OnNunTargetDetectionEndOverlap);

	if (NunAnimInstance != nullptr)
	{
		
	}

	MonsterMoveEventIndex = 1;
}

void ANunMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MinusOpacity)
	{
		OpactiyDeltaTime += 0.01;
		SkeletalMeshComp->SetScalarParameterValueOnMaterials("Opacity", MeshOpacity -= OpactiyDeltaTime);
	}

	//텔레포트 이펙트 확인용
	//사용할 때 텔레포트 함수의 플레이어 락온 부분 주석치고 사용할 것.
	//if (test)
	//{
	//	TelePort();
	//	test = false;
	//}
}

void ANunMonster::SetYaw()
{
	if (PlayerCharacter == nullptr)
		return;

	TargetRotation = (PlayerCharacter->GetActorLocation() - GetActorLocation()).Rotation();
	YawRotation = TargetRotation;
}

void ANunMonster::OnPerceptionTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MonsterController->CanPerception = true;
}

void ANunMonster::OnNunTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (MonsterController->FindPlayer && !CheckDetect && MonsterDataStruct.CharacterHp > 0)
	{
		CheckDetect = true;

		if (MyMonsterType == MonsterType::ILLUSION_NUN)
		{
			ActivateHpBar();
		}
		else
		{
			SpawnKnight(2);
			SelfHealTimer();
			//TelePortAttack();
			GetWorld()->GetTimerManager().SetTimer(TeleportHandle, FTimerDelegate::CreateLambda([=]()
				{
					IsCoolTimeTeleport = true;
					TelePort();
				}), TeleportCoolTime, true, 0.f);
		}

		if (ActionType == MonsterActionType::DEAD)
			return;
		if (PlayerCharacter == nullptr)
		{
			PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		}
	}
}

void ANunMonster::OnNunTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//AttackAnimationType = MonsterAnimationType::NONE;
}

void ANunMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	if (!MonsterController->FindPlayer || IsCoolTime)
		return;

	if (NunAnimInstance == nullptr)
	{
		NunAnimInstance = Cast<UNumAnimInstance>(GetMesh()->GetAnimInstance());
	}

	TracePlayer = false;
	if (StateType == MonsterStateType::CANTACT)
		return;
	AttackAnimationType = AttackAnimType;

	if(CurrentDistance < FirstRange)
		AttackAnimationType = MonsterAnimationType::DARK;
	else if (CurrentDistance >= FirstRange && CurrentDistance <= SecondRange)
		AttackAnimationType = MonsterAnimationType::HEAL1;
	else if(CurrentDistance >= SecondRange)
		AttackAnimationType = MonsterAnimationType::HEAL2;

	if (ActionType != MonsterActionType::ATTACK)
	{
		MonsterController->StopMovement();
		NunAnimInstance->StopMontage(MontageMap[AnimationType]);

		float RandomValue = FMath::RandRange(0, 100) * 0.01f;
		if (SetActionByRandomMap.Contains(AttackAnimationType))
		{
			MonsterMoveEventIndex = 1;
			SetActionByRandomMap[AttackAnimationType](RandomValue);
			IsCoolTime = true;
			GetWorld()->GetTimerManager().SetTimer(PaternDelay, FTimerDelegate::CreateLambda([=]()
				{
					UE_LOG(LogTemp, Warning, TEXT("Cool Time End"));
					IsCoolTime = false;
					StartAttackTrigger(AttackAnimationType);
				}), GetRandNum(MinDelayTime,MaxDelayTime), false);
		}
	}
}

void ANunMonster::EndAttackTrigger(MonsterAnimationType AttackAnimType)
{
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP)
		return;

	//TracePlayer = true;

	AttackAnimationType = AttackAnimType;
}

float ANunMonster::Die(float Dm)
{
	if (MyMonsterType == MonsterType::NUN)
	{
		MonsterController->BossUI->PlayBossDiedAnimtion();
		MonsterController->BossUI->RemoveFromParent();

		//소환한 환영과 기사 삭제
		for (auto SpawnedKnight : KnightArr)
			SpawnedKnight->Die(0.f);

		KnightArr.Empty();

		if (Illusion != nullptr)
			Illusion->Die(0.f);
	}
	else
	{
		auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), OriginNunClass);
		auto OriginNun = Cast<ANunMonster>(actor);
		OriginNun->SelfHeal();

		DeactivateHpBar();
	}

	MonsterController->CanPerception = false;
	CheckDetect = false;
	IsDie = true;
	Imotal = true;
	GetWorld()->GetTimerManager().ClearTimer(SelfHealTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(TeleportHandle);
	GetWorld()->GetTimerManager().ClearTimer(TeleportAttackHandle);

	DeactivateHitCollision();

	NunAnimInstance->StopMontage(MontageMap[AnimationType]);
	ChangeActionType(MonsterActionType::DEAD);
	StateType = MonsterStateType::CANTACT;

	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	ChangeMontageAnimation(MonsterAnimationType::DEAD);

	//머테리얼에 Opacity 값 넣기 전까지 임시로 Visibility 꺼주기
	GetMesh()->SetVisibility(false);

	GetWorld()->GetTimerManager().SetTimer(MonsterDeadTimer, FTimerDelegate::CreateLambda([=]()
		{
			MinusOpacity = true;
		}), 1.2f, false);

	return Dm;
}

void ANunMonster::SpawnKnight(int knightnum)
{
	if (IsIllusion || MonsterDataStruct.CharacterHp < 500)
		return;

	FVector SpawnLoc = FVector::ZeroVector;
	FRotator SpawnRot = FRotator::ZeroRotator;
	FNavLocation RandomLocation;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return;

	for (int i = 0; i < knightnum; i++)
	{
		if (NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), KnightSpawnRadius, RandomLocation))
		{
			FVector Temp = RandomLocation.Location;
			SpawnLoc = FVector(Temp.X, Temp.Y, PlayerCharacter->GetActorLocation().Z);
			//SpawnRot = UKismetMathLibrary::FindLookAtRotation(Knight->GetActorLocation(), PlayerCharacter->GetActorLocation());
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;;
		auto Knight = GetWorld()->SpawnActor<AKinghtMonster>(KnightClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		Knight->SkeletalMeshComp->SetScalarParameterValueOnMaterials("Dither", 0.f);
		Knight->KnightHeadMesh->SetScalarParameterValueOnMaterials("Dither", 0.f);
		Knight->SetActorLocation(SpawnLoc);
		Knight->SetActorRotation(SpawnRot);
		Knight->Imotal = true;
		Knight->MonsterDataStruct.CharacterMaxHp = SpawnedKnightMaxHp;
		Knight->MonsterDataStruct.CharacterHp = SpawnedKnightMaxHp;
		Knight->IsSpawn = true;
		Knight->PlayerCharacter = PlayerCharacter;
		Knight->Super::PlayerCharacter = PlayerCharacter;
		Knight->SpawnBegin();
		Knight->ChangeMontageAnimation(MonsterAnimationType::SPAWNING);
		Knight->MonsterController->FindPlayer = true;
		Knight->ChangeActionType(MonsterActionType::MOVE);
		Knight->MonsterMoveEventIndex = 1;
		KnightArr.Push(Knight);
	}
}

void ANunMonster::MultiHeal()
{
	UE_LOG(LogTemp, Warning, TEXT("MultiHeal"));

	//스피어 콜라이더 만들어서 처리
	TArray<FHitResult> OutHits;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepMultiByChannel(
		OUT OutHits,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel17,
		FCollisionShape::MakeSphere(HealRadius),
		Params);

	FVector Center = GetActorLocation();
	FColor DrawColor;

	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	//DrawDebugSphere(GetWorld(), Center, HealRadius, 16, DrawColor, false, 1.f);

	//CameraShake(PlayerCameraShake);
	//VibrateGamePad(1.0f, 0.5f);

	if (bResult)
	{
		for (auto HitActor : OutHits)
		{
			auto Knight = Cast<AKinghtMonster>(HitActor.GetActor());

			if (Knight == nullptr)
				return;

			if (Knight->MonsterDataStruct.CharacterHp <= 0)
			{
				auto FoundIndex = KnightArr.Find(Knight);

				if (FoundIndex != -1)
				{
					KnightArr.RemoveAtSwap(FoundIndex);
					return;
				}
			}

			if (Knight->MonsterDataStruct.CharacterHp >= Knight->MonsterDataStruct.CharacterMaxHp)
				return;

			Knight->MonsterDataStruct.CharacterHp += MultiHealVal;

			if (Knight->MonsterDataStruct.CharacterHp > Knight->MonsterDataStruct.CharacterMaxHp)
				Knight->MonsterDataStruct.CharacterHp = Knight->MonsterDataStruct.CharacterMaxHp;

			float CurrentPercent = Knight->MonsterDataStruct.CharacterHp / Knight->MonsterDataStruct.CharacterMaxHp;
			Knight->MonsterHPWidget->SetHP(CurrentPercent);

			auto SpawnLoc = Knight->GetActorLocation();

			auto HealPoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
				SpawnLoc + FVector(0, 0, 100), FRotator::ZeroRotator);
			auto HealDustPoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
				SpawnLoc - FVector(0, 0, 65), FRotator::ZeroRotator);
			
			auto HealEffect = Cast<ANunEffectObjInPool>(HealPoolObj);
			auto HealDustEffect = Cast<ANunEffectObjInPool>(HealDustPoolObj);
		
			HealEffect->AttachToComponent(Knight->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			HealDustEffect->AttachToComponent(Knight->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);

			HealEffect->SetCurrentEffect(EffectType::SINGLEHEAL);
			HealDustEffect->SetCurrentEffect(EffectType::HEALDUST);
			
			HealEffect->ActivateCurrentEffect();
			HealDustEffect->ActivateCurrentEffect();
		}
	}
}

void ANunMonster::SelfHealTimer()
{
	GetWorld()->GetTimerManager().SetTimer(SelfHealTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			if (IsDie)
				return;

			if (MonsterDataStruct.CharacterHp >= MonsterDataStruct.CharacterMaxHp)
				return;

			MonsterDataStruct.CharacterHp += SelfHealVal;

			if (MonsterDataStruct.CharacterHp >= MonsterDataStruct.CharacterMaxHp)
				MonsterDataStruct.CharacterHp = MonsterDataStruct.CharacterMaxHp;

			float CurrentPercent = MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp;	
			MonsterController->BossUI->SetHP(CurrentPercent);

			auto SpawnLoc = GetActorLocation();

			auto HealPoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
				SpawnLoc + FVector(0, 0, 200), FRotator::ZeroRotator);
			auto HealDustPoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
				SpawnLoc - FVector(0, 0, 165), FRotator::ZeroRotator);

			auto HealEffect = Cast<ANunEffectObjInPool>(HealPoolObj);
			auto HealDustEffect = Cast<ANunEffectObjInPool>(HealDustPoolObj);

			HealEffect->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			HealDustEffect->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);

			HealEffect->SetCurrentEffect(EffectType::SINGLEHEAL);
			HealDustEffect->SetCurrentEffect(EffectType::HEALDUST);

			HealEffect->ActivateCurrentEffect();
			HealDustEffect->ActivateCurrentEffect();

		}), SelfHealCoolTime, true);
}

void ANunMonster::SelfHeal()
{
	if (IsDie)
		return;

	if (MonsterDataStruct.CharacterHp >= MonsterDataStruct.CharacterMaxHp)
		return;

	MonsterDataStruct.CharacterHp += SelfHealVal;

	if (MonsterDataStruct.CharacterHp >= MonsterDataStruct.CharacterMaxHp)
		MonsterDataStruct.CharacterHp = MonsterDataStruct.CharacterMaxHp;

	float CurrentPercent = MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp;
	MonsterController->BossUI->SetHP(CurrentPercent);

	auto SpawnLoc = GetActorLocation();

	auto HealPoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
		SpawnLoc + FVector(0, 0, 200), FRotator::ZeroRotator);
	auto HealDustPoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
		SpawnLoc - FVector(0, 0, 165), FRotator::ZeroRotator);

	auto HealEffect = Cast<ANunEffectObjInPool>(HealPoolObj);
	auto HealDustEffect = Cast<ANunEffectObjInPool>(HealDustPoolObj);

	HealEffect->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	HealDustEffect->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);

	HealEffect->SetCurrentEffect(EffectType::SINGLEHEAL);
	HealDustEffect->SetCurrentEffect(EffectType::HEALDUST);

	HealEffect->ActivateCurrentEffect();
	HealDustEffect->ActivateCurrentEffect();
}

void ANunMonster::DotFloor()
{
	UE_LOG(LogTemp, Warning, TEXT("DotFloor"));

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return;

	FNavLocation RandomLocation;

	for (int i = 0; i < 3; i++)
	{
		if (NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), DotRange, RandomLocation))
		{
			FVector Temp = RandomLocation.Location;
			auto Loc = FVector(Temp.X, Temp.Y, PlayerCharacter->GetActorLocation().Z - 87.f);

			auto PoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
				Loc, FRotator::ZeroRotator);
			
			auto NunEffect = Cast<ANunEffectObjInPool>(PoolObj);
			NunEffect->DamageSphereTriggerComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			NunEffect->DamageSphereTriggerComp->Count = 10000;
			if (IsIllusion)
				NunEffect->DamageSphereTriggerComp->Damage = 0.f;
			else
				NunEffect->DamageSphereTriggerComp->Damage = 20.f;
			NunEffect->DamageSphereTriggerComp->DamageTime = 1.f;
			NunEffect->SetCurrentEffect(EffectType::WORSHIPEFFECT);
			NunEffect->ActivateCurrentEffect();
			NunEffect->DeactivateDamageSphere(DotTime);
		}
	}
}

void ANunMonster::JudementAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("JudementAttack"));

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return;

	GetWorld()->GetTimerManager().SetTimer(JudementTimer, FTimerDelegate::CreateLambda([=]()
		{
			if (!MonsterController->FindPlayer)
				return;

			FNavLocation RandomLocation;
			if (NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), JudementRange, RandomLocation))
			{
				FVector Temp = RandomLocation.Location;
				auto Loc = FVector(Temp.X, Temp.Y, PlayerCharacter->GetActorLocation().Z - 87.f);
				auto PoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
					Loc, FRotator::ZeroRotator);
				
				auto JudementObj = Cast<ANunEffectObjInPool>(PoolObj);
				JudementObj->SetCurrentEffect(EffectType::JUDGEMENTEFFECT);
				JudementObj->ActivateCurrentEffect();
				JudementObj->Damage = 20;
				JudementObj->SweepSingle(1.f, JudementProjectileRange, SkillInfoMap[MonsterAnimationType::JUDEMENT].Damage,
					IsIllusion, GetController());
				
				JudementCurrentCount++;
			}

			if (JudementCurrentCount >= JudementMaxCount)
			{
				JudementCurrentCount = 0;
				GetWorld()->GetTimerManager().ClearTimer(JudementTimer);
				return;
			}
		}), JudementDelay, true);
}

void ANunMonster::Curse()
{
	UE_LOG(LogTemp, Warning, TEXT("Curse"));

}

void ANunMonster::CrystalAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("CrystalAttack"));

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return;

	FNavLocation RandomLocation;

	auto PlayerLoc = PlayerCharacter->GetActorLocation();

	auto CrystalStart = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
		CrystalSpawnLoc->GetComponentLocation(), FRotator::ZeroRotator);
	auto CrystalStartEffect = Cast<ANunEffectObjInPool>(CrystalStart);
	CrystalStartEffect->SetCurrentEffect(EffectType::CRYSTALEFFECT_START);
	CrystalStartEffect->ActivateCurrentEffect();

	for (int i = 0; i < CrystalCount; i++)
	{
		if (NavSystem->GetRandomPointInNavigableRadius(PlayerLoc, CrystalRange, RandomLocation))
		{
			FVector Temp = RandomLocation.Location;

			auto PoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
				CrystalSpawnLoc->GetComponentLocation(), FRotator::ZeroRotator);
			auto CrystalEffect = Cast<ANunEffectObjInPool>(PoolObj);

			if (IsIllusion)
				CrystalEffect->Damage = 0.f;
			CrystalEffect->SetCurrentEffect(EffectType::CRYSTALEFFECT);
			CrystalEffect->ActivateCurrentEffect();
			CrystalEffect->ShotProjectile(Temp);
			CrystalEffect->SetActorTickEnabled(true);
		}
	}
}

void ANunMonster::FogAttack()
{
	auto Loc = GetActorLocation();

	auto PoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
		GetActorLocation(), FRotator::ZeroRotator);
	auto FogEffect = Cast<ANunEffectObjInPool>(PoolObj);

	FogEffect->SetCurrentEffect(EffectType::FOGEFFECT);
	FogEffect->ActivateCurrentEffect();
	FogEffect->DamageSphereTriggerComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FogEffect->DamageSphereTriggerComp->MaxCount = 1;
	if (IsIllusion)
		FogEffect->DamageSphereTriggerComp->Damage = 0.f;
	else
		FogEffect->DamageSphereTriggerComp->Damage = 50.f;
	FogEffect->DamageSphereTriggerComp->DamageTime = 1.f;
	FogEffect->DeactivateDamageSphere(1.f);
}

void ANunMonster::PrayAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("PrayAttack"));

	if (SpawnLocArr.IsEmpty())
		return;

	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			auto DarkPoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
			SpawnLocArr[PraySpawnCount]->GetComponentLocation(), FRotator::ZeroRotator);

			auto PrayObj = Cast<ANunEffectObjInPool>(DarkPoolObj);
			PrayObj->SetCurrentEffect(EffectType::PRAYEFFECT);
			PrayObj->ActivateCurrentEffect();
			PrayObj->SetActorTickEnabled(true);
			PrayObj->ShotProjectile(true, PlayerCharacter->GetActorLocation());
			PrayObj->Delay = PrayDelay;
			PrayObj->Speed = 1000.f;
			if (IsIllusion)
				PrayObj->Damage = 0.f;
			else
				PrayObj->Damage = PrayDamage;
			++PraySpawnCount;

			if (PraySpawnCount >= SpawnLocArr.Num())
			{
				PraySpawnCount = 0;
				GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
			}
		}), PrayObjSpawnDelay, true);
}

void ANunMonster::FragmentsAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("FragmentsAttack"));

	auto Loc = GetActorLocation();

	auto PoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
		FVector(Loc.X,Loc.Y,Loc.Z-160.f), FRotator::ZeroRotator);

	auto FragmentObj = Cast<ANunEffectObjInPool>(PoolObj);
	FragmentObj->SetCurrentEffect(EffectType::FRAGMENTEFFECT_BURST);
	FragmentObj->ActivateCurrentEffect();

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(FragmentRange),
		Params);

	FColor DrawColor;

	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	//DrawDebugSphere(GetWorld(), GetActorLocation(), FragmentRange, 16, DrawColor, false, 2.f);

	//CameraShake(PlayerCameraShake);
	//VibrateGamePad(1.0f, 0.5f);

	if (bResult && HitResult.GetActor())
	{
		FDamageEvent DamageEvent;
		auto Player = Cast<APlayerCharacter>(HitResult.GetActor());

		if (IsIllusion || Player == nullptr)
			return;

		Player->TakeDamage(SkillInfoMap[MonsterAnimationType::FRAGMENT].Damage, DamageEvent, GetController(), this);
	}
}

void ANunMonster::IllusionAttack()
{
	if (useIllusion || IsIllusion)
		return;

	UE_LOG(LogTemp, Warning, TEXT("IllusionAttack"));

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;;
	FVector SpawnLoc = FVector::ZeroVector;
	FRotator SpawnRot = FRotator::ZeroRotator;

	Illusion = GetWorld()->SpawnActor<ANunMonster>(IllusionNunClass, SpawnLoc, SpawnRot, SpawnParams);

	UE_LOG(LogTemp, Warning, TEXT("illusion CurrentNum = %d"), CurrentNum);

	srand(time(NULL));
	int Num = rand() % TeleportArr.Num();

	while (1)
	{
		if (CurrentNum != Num)
			break;

		srand(time(NULL));
		Num = rand() % TeleportArr.Num();
	}

	Illusion->SetActorLocation(TeleportArr[Num]->GetActorLocation());
	Illusion->SetActorRotation(SpawnRot);
	Illusion->MonsterController->FindPlayer = true;
	Illusion->IsIllusion = true;
	Illusion->PlayerCharacter = PlayerCharacter;
	useIllusion = true;
	Illusion->SetYaw();

	GetWorld()->GetTimerManager().SetTimer(IllusionTimer, FTimerDelegate::CreateLambda([=]()
		{
			Illusion->MonsterDataStruct.CharacterHp = -1;
			Illusion->Die(0.f);
			useIllusion = false;
			GetWorld()->GetTimerManager().ClearTimer(IllusionTimer);
		}), IllusionTime, false);
}

void ANunMonster::DarkAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("DarkProjectile"));
	if (SpawnLocArr.IsEmpty())
		return;

	int RandomValue = FMath::RandRange(0, SpawnLocArr.Num() - 1);

	auto DarkPoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
		SpawnLocArr[RandomValue]->GetComponentLocation(), FRotator::ZeroRotator);

	auto DarkObj = Cast<ANunEffectObjInPool>(DarkPoolObj);

	if (IsIllusion)
		DarkObj->Damage = 0.f;

	DarkObj->SetCurrentEffect(EffectType::DARKEFFECT);
	DarkObj->Delay = DarkDelay;
	DarkObj->ActivateCurrentEffect();
	DarkObj->ShotProjectile(PlayerCharacter);
	DarkObj->SetActorTickEnabled(true);
}

void ANunMonster::SingleHeal()
{
	UE_LOG(LogTemp, Warning, TEXT("SingleHeal"));

	if (KnightArr.IsEmpty())
		return;

	TArray<float> KnightHpArr;
	int Min = 0;
	int index = 0;
	int RemoveIndex = -1;

	for (int i = 0; i < KnightArr.Num(); i++)
	{
  		if (KnightArr[i]->MonsterDataStruct.CharacterHp <= 0)
		{
			RemoveIndex = KnightArr.Find(KnightArr[i]);
			KnightArr.RemoveAtSwap(RemoveIndex);
		}
	}

	for (int i = 0; i < KnightArr.Num(); i++)
	{
		KnightHpArr.Push(KnightArr[i]->MonsterDataStruct.CharacterHp);
	}

	if (KnightArr.IsEmpty())
		return;

	Min = KnightHpArr[0];

	for (int i = 0; i < KnightHpArr.Num(); i++)
	{
		if (Min > KnightHpArr[i])
		{
			Min = KnightHpArr[i];
			index = i;
		}
	}

	auto TargetKnight = KnightArr[index];

	if (TargetKnight->MonsterDataStruct.CharacterHp >= TargetKnight->MonsterDataStruct.CharacterMaxHp)
		return;

	TargetKnight->MonsterDataStruct.CharacterHp += HealVal;

	if (TargetKnight->MonsterDataStruct.CharacterHp > TargetKnight->MonsterDataStruct.CharacterMaxHp)
		TargetKnight->MonsterDataStruct.CharacterHp = TargetKnight->MonsterDataStruct.CharacterMaxHp;

	float CurrentPercent = TargetKnight->MonsterDataStruct.CharacterHp / TargetKnight->MonsterDataStruct.CharacterMaxHp;
	TargetKnight->MonsterHPWidget->SetHP(CurrentPercent);

	KnightHpArr.Empty();

	auto SpawnLoc = TargetKnight->GetActorLocation();

	auto HealPoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
		SpawnLoc + FVector(0, 0, 100), FRotator::ZeroRotator);
	auto HealDustPoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
		SpawnLoc - FVector(0, 0, 65), FRotator::ZeroRotator);

	auto HealEffect = Cast<ANunEffectObjInPool>(HealPoolObj);
	auto HealDustEffect = Cast<ANunEffectObjInPool>(HealDustPoolObj);

	HealEffect->AttachToComponent(TargetKnight->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	HealDustEffect->AttachToComponent(TargetKnight->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);

	HealEffect->SetCurrentEffect(EffectType::SINGLEHEAL);
	HealDustEffect->SetCurrentEffect(EffectType::HEALDUST);

	HealEffect->ActivateCurrentEffect();
	HealDustEffect->ActivateCurrentEffect();
}

void ANunMonster::Stun()
{
	NunAnimInstance->StopMontage(MontageMap[AnimationType]);
	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	ChangeMontageAnimation(MonsterAnimationType::DEAD);
}

void ANunMonster::MonsterHitStop()
{
	if (MontageMap.Contains(AnimationType))
		NunAnimInstance->PauseAnimation(MontageMap[AnimationType]);
}

void ANunMonster::ChangeMontageAnimation(MonsterAnimationType type)
{
	if (NunAnimInstance == nullptr)
	{
		NunAnimInstance = Cast<UNumAnimInstance>(GetMesh()->GetAnimInstance());
	}
	else
	{
		NunAnimInstance->StopMontage(MontageMap[AnimationType]);
		AnimationType = type;
		StateType = AnimTypeToStateType[type];
		NunAnimInstance->PlayMontage(MontageMap[type]);
	}
}

float ANunMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ChangeActionType(MonsterActionType::NONE);
	if (Imotal)
	{
		return 0;
	}
	DeactivateHitCollision();
	MonsterDataStruct.CharacterHp -= DamageAmount;

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ChangeMontageAnimation(MonsterAnimationType::HIT);

	if (MyMonsterType == MonsterType::NUN)
	{
		MonsterController->BossUI->DecreaseHPGradual(this, MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp);
		MonsterController->BossUI->SetDamageText(DamageAmount);
	}
	else
	{
		MonsterHPWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		float CurrentPercent = MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp;
		MonsterHPWidget->DecreaseHPGradual(this, CurrentPercent);
	}

	TeleportDamageSum += DamageAmount;
	TeleportAttackDamageSum += DamageAmount;
	SpawnDamageSum += DamageAmount;
	IllusionDamageSum += DamageAmount;

	if (TeleportDamageSum >= MonsterDataStruct.CharacterMaxHp * TeleportVal)
	{
		if (IsIllusion)
			return 0.f;
		TelePort();
		TeleportDamageSum = 0;
	}

	if (TeleportAttackDamageSum >= MonsterDataStruct.CharacterMaxHp * TeleportAttackVal)
	{
		if (IsIllusion)
			return 0.f;
		TelePortAttack();
		TeleportAttackDamageSum = 0;
		UE_LOG(LogTemp, Warning, TEXT("TeleportAttack"));
		return 0.f;
	}

	if (SpawnDamageSum >= MonsterDataStruct.CharacterMaxHp * KnightSpawnVal)
	{
		if (SpawnLevel > MaxSpawnLevel)
			return 0.f;

		SpawnKnight(KnightSpawnMap[SpawnLevel]);
		SpawnDamageSum = 0;
	}
	if (IllusionDamageSum >= MonsterDataStruct.CharacterMaxHp * IllusionVal)
	{
		IllusionAttack();
		IllusionDamageSum = 0;
	}

	return DamageAmount;
}

void ANunMonster::SpawnMagicCircle()
{
	if (IsIllusion)
		return;

	auto Loc = GetActorLocation();

	auto Obj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
		FVector(Loc.X, Loc.Y, Loc.Z - 160.f), FRotator::ZeroRotator);

	Obj->SetActorScale3D(FVector(3.5f, 3.5f, 1.f));
	auto MagicCircleObj = Cast<ANunEffectObjInPool>(Obj);
	MagicCircleObj->SetCurrentEffect(EffectType::MAGICCIRCLE);
	MagicCircleObj->ActivateCurrentEffect();

	MagicCircleObj->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ANunMonster::TelePort()
{
	if (PlayerCharacter->IsLockOn)
		PlayerCharacter->LockOn();

	//사라진 상태에서 0.7초후에 이동시키고 메쉬 활성화

	auto TeleportInObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
		GetActorLocation(), FRotator::ZeroRotator);
	auto Temp1 = Cast<ANunEffectObjInPool>(TeleportInObj);
	Temp1->SetCurrentEffect(EffectType::TELEPORT_IN);
	Temp1->ActivateCurrentEffect();

	SetActive(false);
	GetMesh()->SetVisibility(false);
	DeactivateHitCollision();

	GetWorld()->GetTimerManager().SetTimer(TeleportTimer, FTimerDelegate::CreateLambda([=]()
		{
			srand(time(NULL));
			auto Num = rand() % TeleportArr.Num();
			while (1)
			{
				if (CurrentNum != Num)
					break;

				srand(time(NULL));
				Num = rand() % TeleportArr.Num();
			}
			CurrentNum = Num;

			//UE_LOG(LogTemp, Warning, TEXT("origin CurrentNum = %d"), CurrentNum);

			SetActorLocation(TeleportArr[Num]->GetActorLocation());

			SetActive(true);
			GetMesh()->SetVisibility(true);
			ActivateHitCollision();

			auto TeleportOutObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
				GetActorLocation(), FRotator::ZeroRotator);
			auto Temp2 = Cast<ANunEffectObjInPool>(TeleportOutObj);
			Temp2->SetCurrentEffect(EffectType::TELEPORT_OUT);
			Temp2->ActivateCurrentEffect();

			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
			FogAttack();
			SetYaw();
			GetWorld()->GetTimerManager().ClearTimer(TeleportTimer);

			if (IsCoolTimeTeleport)
			{
				if (Count < 1)
				{
					++Count;
					TelePort();
				}
				else
				{
					IsCoolTimeTeleport = false;
					Count = 0;
				}
			}
		}), TeleportDelayVal, false);
}

void ANunMonster::TelePortAttack()
{
	DarkAttack();

	GetWorld()->GetTimerManager().PauseTimer(TeleportHandle);
	GetWorld()->GetTimerManager().PauseTimer(PaternDelay);

	GetWorld()->GetTimerManager().SetTimer(TeleportAttackHandle, FTimerDelegate::CreateLambda([=]()
		{
			TelePortTempFunc();
			GetWorld()->GetTimerManager().SetTimer(TeleportAttackHandle, FTimerDelegate::CreateLambda([=]()
				{
					DarkAttack();
					GetWorld()->GetTimerManager().SetTimer(TeleportAttackHandle, FTimerDelegate::CreateLambda([=]()
						{
							TelePortTempFunc();
							GetWorld()->GetTimerManager().SetTimer(TeleportAttackHandle, FTimerDelegate::CreateLambda([=]()
								{
									CrystalAttack();
									GetWorld()->GetTimerManager().UnPauseTimer(TeleportHandle);
									GetWorld()->GetTimerManager().UnPauseTimer(PaternDelay);
								}), 0.5f, false, 0.5f);
						}), 2.5f, false, 2.5f);
				}), 0.5f, false, 0.5f);
		}), 2.5f, false, 2.5f);
}

void ANunMonster::TelePortTempFunc()
{
	SetActive(false);
	GetMesh()->SetVisibility(false);
	DeactivateHitCollision();

	srand(time(NULL));
	auto Num = rand() % TeleportArr.Num();
	while (1)
	{
		if (CurrentNum != Num)
			break;

		srand(time(NULL));
		Num = rand() % TeleportArr.Num();
	}
	CurrentNum = Num;

	SetActorLocation(TeleportArr[Num]->GetActorLocation());

	SetActive(true);
	GetMesh()->SetVisibility(true);
	ActivateHitCollision();

	auto TeleportOutObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
		GetActorLocation(), FRotator::ZeroRotator);
	auto Temp2 = Cast<ANunEffectObjInPool>(TeleportOutObj);
	Temp2->SetCurrentEffect(EffectType::TELEPORT_OUT);
	Temp2->ActivateCurrentEffect();

	ChangeActionType(MonsterActionType::NONE);
	ChangeMontageAnimation(MonsterAnimationType::IDLE);
	FogAttack();
	SetYaw();
}

void ANunMonster::CheckMontageEndNotify()
{
	if (MontageEndEventMap.Contains(AnimationType))
	{
		MontageEndEventMap[AnimationType]();
	}
}

void ANunMonster::PlayExecutionAnimation()
{
	IsStun = false;
	CanExecution = false;
	ChangeMontageAnimation(MonsterAnimationType::EXECUTION);
}

void ANunMonster::IsNotifyActive(bool value)
{
	if (NotifyBeginEndEventMap.Contains(AnimationType))
	{
		NotifyBeginEndEventMap[AnimationType][value]();
	}
}

void ANunMonster::RespawnCharacter()
{
	if (MyMonsterType == MonsterType::ILLUSION_NUN)
	{
		auto index = UCombatManager::GetInstance().HitMonsterInfoArray.Find(this);
		UCombatManager::GetInstance().HitMonsterInfoArray.RemoveAt(index);
		SetActorTickEnabled(false);
		GetWorld()->DestroyActor(this);
		return;
	}

	Super::RespawnCharacter();
	UE_LOG(LogTemp, Warning, TEXT("nun respawn"));

	GetWorld()->GetTimerManager().ClearTimer(TeleportHandle);
	
	for (auto Knight : KnightArr)
		Knight->RespawnCharacter();

	CheckDetect = false;
	MonsterController->CanPerception = false;
	MonsterController->FindPlayer = false;

	KnightArr.Empty();
	PlayerCharacter = nullptr;

	TeleportDamageSum = 0.f;
	SpawnDamageSum = 0.f;
	IllusionDamageSum = 0.f;
	SpawnLevel = 1;

	MeshOpacity = 1.0f;

	MinusOpacity = false;

	//머테리얼에 Opacity 값 넣기 전까지 임시로 Visibility 꺼주기
	GetMesh()->SetVisibility(true);

	GetCapsuleComponent()->SetCollisionProfileName("AIPhysics");
	SkeletalMeshComp->SetScalarParameterValueOnMaterials("Dither", MeshOpacity);

	ActivateHitCollision();
	MonsterDataStruct.CharacterHp = MonsterDataStruct.CharacterMaxHp;
	MonsterController->BossUI->SetHP(1);
	MonsterController->BossUI->RemoveFromParent();

	ChangeActionType(MonsterActionType::NONE);
	ChangeMontageAnimation(MonsterAnimationType::IDLE);
}

void ANunMonster::ResumeMontage()
{
	if (MontageMap.Contains(AnimationType))
		NunAnimInstance->ResumeMontage(MontageMap[AnimationType]);
}
