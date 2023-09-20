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

ANunMonster::ANunMonster()
{
	AttackTrigger = CreateDefaultSubobject<UNunAttackTriggerComp>(TEXT("AttackTriggerCollision"));
	AttackTrigger->SetupAttachment(GetMesh());

	ProjectileRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileRootComp"));
	ProjectileRootComp->SetupAttachment(RootComponent);

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
	
	NotifyBeginEndEventMap.Add(MonsterAnimationType::HEAL1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::HEAL1].Add(true, [&]()
		{
			SingleHeal();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::HEAL1].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::HEAL2, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::HEAL2].Add(true, [&]()
		{
			MultiHeal();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::HEAL2].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::SPAWNKNIGHT, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::SPAWNKNIGHT].Add(true, [&]()
		{
			SpawnKnight();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::SPAWNKNIGHT].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::WORSHIP, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::WORSHIP].Add(true, [&]()
		{
			DotFloor();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::WORSHIP].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::JUDEMENT, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::JUDEMENT].Add(true, [&]()
		{
			JudementAttack();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::JUDEMENT].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::CURSE, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::CURSE].Add(true, [&]()
		{
			Curse();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::CURSE].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::SELFHEAL, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::SELFHEAL].Add(true, [&]()
		{
			UE_LOG(LogTemp, Warning, TEXT("SelfHeal"));

			if (MonsterDataStruct.CharacterHp >= MonsterDataStruct.CharacterMaxHp)
				return;

			MonsterDataStruct.CharacterHp += SelfHealVal;

			if (MonsterDataStruct.CharacterHp >= MonsterDataStruct.CharacterMaxHp)
				MonsterDataStruct.CharacterHp = MonsterDataStruct.CharacterMaxHp;

			float CurrentPercent = MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp;
			MonsterHPWidget->SetHP(CurrentPercent);

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
		});
	NotifyBeginEndEventMap[MonsterAnimationType::SELFHEAL].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::DARK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::DARK].Add(true, [&]()
		{
			UE_LOG(LogTemp, Warning, TEXT("DarkProjectile"));
			if (SpawnLocArr.IsEmpty())
				return;

			int RandomValue = FMath::RandRange(0, SpawnLocArr.Num() - 1);

			auto DarkPoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
				SpawnLocArr[RandomValue]->GetComponentLocation(), FRotator::ZeroRotator);

			auto DarkObj = Cast<ANunEffectObjInPool>(DarkPoolObj);
			DarkObj->SetCurrentEffect(EffectType::DARKEFFECT);
			DarkObj->ActivateCurrentEffect();
			DarkObj->ShotProjectile(PlayerCharacter);
			DarkObj->SetActorTickEnabled(true);
		});

	NotifyBeginEndEventMap[MonsterAnimationType::DARK].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::PRAY, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::PRAY].Add(true, [&]()
		{
		});
	NotifyBeginEndEventMap[MonsterAnimationType::PRAY].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::FRAGMENT, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::FRAGMENT].Add(true, [&]()
		{
			FragmentsAttack();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::FRAGMENT].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::CRYSTAL, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::CRYSTAL].Add(true, [&]()
		{
			CrystalAttack();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::CRYSTAL].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::ILLUSION, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::ILLUSION].Add(true, [&]()
		{
			IllusionAttack();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::ILLUSION].Add(false, [&]()
		{
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
			if (percent <= 0.4)
			{
				//파편
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::FRAGMENT);
			}
			else if (percent > 0.3f && percent < 0.7f)
			{
				//심판
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::CRYSTAL);
			}
			else
			{
				//저주
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::IDLE);
				FogAttack();
			}
		});

	SetActionByRandomMap.Add(MonsterAnimationType::HEAL1, [&](float percent)
		{
			if (percent <= 0.4)
			{
				//단일힐
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::HEAL1);
			}
			else if (percent > 0.4f && percent < 0.8f)
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
			if (percent <= 0.3)
			{
				//어둠
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::DARK);
			}
			else if (percent > 0.3f && percent < 0.5f)
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
	TargetRotation = (PlayerCharacter->GetActorLocation() - GetActorLocation()).Rotation();
	YawRotation = TargetRotation;
}

void ANunMonster::OnNunTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ActivateHpBar();

	SelfHeal();

	if (ActionType == MonsterActionType::DEAD)
		return;
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	}
}

void ANunMonster::OnNunTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//AttackAnimationType = MonsterAnimationType::NONE;
}

void ANunMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	UE_LOG(LogTemp, Warning, TEXT("StartAttackTrigger : %f"), CurrentDistance);

	if (!MonsterController->FindPlayer)
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
			UE_LOG(LogTemp, Warning, TEXT("SetActionByRandomMap"));
			MonsterMoveEventIndex = 1;
			SetActionByRandomMap[AttackAnimationType](RandomValue);
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
	if (MonsterDataStruct.CharacterHp <= 0)
	{
		IsDie = true;
		Imotal = true;
		GetWorld()->GetTimerManager().ClearTimer(SelfHealTimerHandle);

		DeactivateHpBar();
		DeactivateHitCollision();

		NunAnimInstance->StopMontage(MontageMap[AnimationType]);
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
			}), 1.2f, false);

		return Dm;
	}

	return Dm;
}

void ANunMonster::SpawnKnight()
{
	if (IsIllusion)
		return;

	for (int i = 0; i < KnightNum; i++)
	{
		FActorSpawnParameters SpawnParams; 
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;;
		FVector SpawnLoc = FVector::ZeroVector;
		FRotator SpawnRot = FRotator::ZeroRotator;

		auto Knight = GetWorld()->SpawnActor<AKinghtMonster>(KnightClass,SpawnLoc,SpawnRot,SpawnParams);

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (NavSystem == nullptr)
			return;

		FNavLocation RandomLocation;

		if (NavSystem->GetRandomPointInNavigableRadius(PlayerCharacter->GetActorLocation(), KnightSpawnRadius, RandomLocation))
		{
			FVector Temp = RandomLocation.Location;
			SpawnLoc = FVector(Temp.X, Temp.Y, PlayerCharacter->GetActorLocation().Z);
			//SpawnRot = UKismetMathLibrary::FindLookAtRotation(Knight->GetActorLocation(), PlayerCharacter->GetActorLocation());
		}

		Knight->SetActorLocation(SpawnLoc);
		Knight->SetActorRotation(SpawnRot);
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

	DrawDebugSphere(GetWorld(), Center, HealRadius, 16, DrawColor, false, 1.f);

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
					KnightArr.RemoveAt(FoundIndex);
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

void ANunMonster::SelfHeal()
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
			MonsterHPWidget->SetHP(CurrentPercent);

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

	}), SelfHealCoolTime,true);
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
			NunEffect->DamageSphereTriggerComp->bHiddenInGame = false;
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
		JudementObj->ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
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

	auto PoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[41].ObjClass,
		GetActorLocation(), FRotator::ZeroRotator);

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

	DrawDebugSphere(GetWorld(), GetActorLocation(), FragmentRange, 16, DrawColor, false, 2.f);

	//CameraShake(PlayerCameraShake);
	//VibrateGamePad(1.0f, 0.5f);

	if (bResult && HitResult.GetActor())
	{
		FDamageEvent DamageEvent;
		auto Player = Cast<APlayerCharacter>(HitResult.GetActor());

		if (IsIllusion)
			return;

		Player->TakeDamage(SkillInfoMap[MonsterAnimationType::FRAGMENT].Damage, DamageEvent, GetController(), this);
	}
}

void ANunMonster::IllusionAttack()
{
	if (IsIllusion)
		return;

	UE_LOG(LogTemp, Warning, TEXT("IllusionAttack"));

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;;
	FVector SpawnLoc = FVector::ZeroVector;
	FRotator SpawnRot = FRotator::ZeroRotator;

	auto Illusion = GetWorld()->SpawnActor<ANunMonster>(IllusionNunClass, SpawnLoc, SpawnRot, SpawnParams);

	srand(time(NULL));
	int Num = rand() % TeleportArr.Num();

	while (CurrentNum == Num)
	{
		srand(time(NULL));
		Num = rand() % TeleportArr.Num();
	}

	Illusion->SetActorLocation(TeleportArr[Num]->GetActorLocation());
	Illusion->SetActorRotation(SpawnRot);
	Illusion->MonsterController->FindPlayer = true;
	Illusion->IsIllusion = true;
	IsIllusion = true;
	Illusion->SetYaw();

	GetWorld()->GetTimerManager().SetTimer(IllusionTimer, FTimerDelegate::CreateLambda([=]()
		{
			Illusion->MonsterDataStruct.CharacterHp = -1;
			Illusion->Die(0.f);
			IsIllusion = false;
			GetWorld()->GetTimerManager().ClearTimer(IllusionTimer);
		}), IllusionTime, false);
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
			RemoveIndex = KnightArr.Find(KnightArr[i]);
		else 		
			KnightHpArr.Push(KnightArr[i]->MonsterDataStruct.CharacterHp);
	}

	if (RemoveIndex != -1)
	{
		KnightArr.RemoveAt(RemoveIndex);
		KnightHpArr.RemoveAt(RemoveIndex);
	}

	if (*KnightArr.begin() == nullptr)
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
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	ChangeActionType(MonsterActionType::NONE);

	if (Imotal)
	{
		return 0;
	}

	//MonsterHpWidget->Hp->SetVisibility(ESlateVisibility::Visible);
	//MonsterHpWidget->HpBG->SetVisibility(ESlateVisibility::Visible);
	//GetWorldTimerManager().SetTimer(HpTimer, this, &AEnemyMonster::DeactivateHpBar, 3.0f);

	DeactivateHitCollision();

	MonsterHPWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	MonsterDataStruct.CharacterHp -= DamageAmount;

	float CurrentPercent = MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp;
	MonsterHPWidget->DecreaseHPGradual(this, CurrentPercent);

	Die(DamageAmount);

	TeleportDamageSum += DamageAmount;
	SpawnDamageSum += DamageAmount;
	IllusionDamageSum += DamageAmount;

	if (TeleportDamageSum >= MonsterDataStruct.CharacterMaxHp * TeleportVal)
	{
		if (IsIllusion)
			return 0.f;
		TelePort();
		TeleportDamageSum = 0;
	}
	if (SpawnDamageSum >= MonsterDataStruct.CharacterMaxHp * KnightSpawnVal)
	{
		SpawnKnight();
		SpawnDamageSum = 0;
	}
	if (IllusionDamageSum >= MonsterDataStruct.CharacterMaxHp * IllusionVal)
	{
		IllusionAttack();
		IllusionDamageSum = 0;
	}

	return DamageAmount;
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
			GetWorld()->GetTimerManager().ClearTimer(TeleportTimer);
		}), TeleportDelayVal, false);
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
	Super::RespawnCharacter();

	WeaponOpacity = 0.171653f;
	MeshOpacity = 0.171653f;

	IsDie = false;
	SelfHeal();

	MinusOpacity = false;

	DeactivateHpBar();
	ActivateHitCollision();
	MonsterDataStruct.CharacterHp = MonsterDataStruct.CharacterMaxHp;
	MonsterHPWidget->SetHP(1.0f);
}

void ANunMonster::ResumeMontage()
{
	if (MontageMap.Contains(AnimationType))
		NunAnimInstance->ResumeMontage(MontageMap[AnimationType]);
}
