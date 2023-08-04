// Fill out your copyright notice in the Description page of Project Settings.


#include "JesusBoss2.h"
#include "..\Player\PlayerCharacter.h"
#include <random>
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "..\Manager\CombatManager.h"
#include "..\ObjectPool\VomitObjectInPool.h"
#include "..\ObjectPool\StoneObjectInPool.h"
#include "DrawDebugHelpers.h"
#include "..\Manager\JesusThreadManager.h"

AJesusBoss2::AJesusBoss2()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Boss2HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Boss2 Hit Collision"));
	Boss2HitCollision->SetupAttachment(GetMesh(), FName("LockOn_Bone"));
	Boss2HitCollision->SetCollisionProfileName("AIHit");

	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOn Widget"));
	LockOnWidget->SetupAttachment(GetMesh(), FName("Bip001-Head"));
	
	LeftAtkCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Boss Left Hand"));
	LeftAtkCollision->SetupAttachment(GetMesh(),FName("LHand"));
	LeftAtkCollision->SetCollisionProfileName("AIWeapon");

	RightAtkCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Boss Right Hand"));
	RightAtkCollision->SetupAttachment(GetMesh(),FName("RHand"));
	RightAtkCollision->SetCollisionProfileName("AIWeapon");

	HeadAtkCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Boss Head"));
	HeadAtkCollision->SetupAttachment(GetMesh(), FName("Bip001-Head"));
	HeadAtkCollision->SetCollisionProfileName("AIWeapon");

	ChargeAtkCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Boss Charge"));
	ChargeAtkCollision->SetupAttachment(GetMesh(), FName("Bip001-Head"));
	ChargeAtkCollision->SetCollisionProfileName("AIWeapon");

	AreaAtkPos = CreateDefaultSubobject<USceneComponent>(TEXT("AreaAtkPos"));
	AreaAtkPos->SetupAttachment(GetMesh());
	AreaAtkPos->SetupAttachment(GetMesh(), FName("RHand"));

	HeadHitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadHitCollision"));
	HeadHitCollision->SetupAttachment(GetMesh(), FName("Bip001-Head"));
	HeadHitCollision->SetCollisionProfileName("AIHit");

	LeftArmHitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftArmHitCollision"));
	LeftArmHitCollision->SetupAttachment(GetMesh(), FName("Bip001-L-UpperArm"));
	LeftArmHitCollision->SetCollisionProfileName("AIHit");

	RightArmHitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightArmHitCollision"));
	RightArmHitCollision->SetupAttachment(GetMesh(), FName("Bip001-R-UpperArm"));
	RightArmHitCollision->SetCollisionProfileName("AIHit");

	JumpExplosonCollider = CreateDefaultSubobject<USphereComponent>(TEXT("JumpExplosionCollision"));
	JumpExplosonCollider->SetupAttachment(GetMesh());
	JumpExplosonCollider->SetCollisionProfileName("AIWeapon");


	MontageStartMap.Add(Boss2AnimationType::NONE, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
		}));
	MontageEndMap.Add(Boss2AnimationType::NONE, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
		}));

	MontageStartMap.Add(Boss2AnimationType::IDLE, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
		}));
	MontageEndMap.Add(Boss2AnimationType::IDLE, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->IsAttackMontageEnd = true;
			Boss2->IsAttacking = false;
		}));

	MontageStartMap.Add(Boss2AnimationType::CROSSFALL, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->IsAttackMontageEnd = false;
			Boss2->CanMove = false;
		}));
	MontageEndMap.Add(Boss2AnimationType::CROSSFALL, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->CanMove = true;
			Boss2->IsLockOn = true;
		}));

	MontageStartMap.Add(Boss2AnimationType::FOWARDWALK, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
		}));
	MontageEndMap.Add(Boss2AnimationType::FOWARDWALK, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
		}));

	MontageStartMap.Add(Boss2AnimationType::LEFTWALK, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
		}));
	MontageEndMap.Add(Boss2AnimationType::LEFTWALK, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
		}));

	MontageStartMap.Add(Boss2AnimationType::SLASH, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->IsAttackMontageEnd = false;
			Boss2->CanMove = false;
		}));
	MontageEndMap.Add(Boss2AnimationType::SLASH, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->CanMove = true;
			Boss2->IsLockOn = true;
		}));

	MontageStartMap.Add(Boss2AnimationType::DOWNSMASH, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->AreaAtkPos->AttachToComponent(Boss2->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RHand"));
			Boss2->IsAttackMontageEnd = false;
			Boss2->CanMove = false;
		}));
	MontageEndMap.Add(Boss2AnimationType::DOWNSMASH, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->CanMove = true;
			Boss2->IsLockOn = true;
		}));

	MontageStartMap.Add(Boss2AnimationType::DOUBLESMASH, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->IsAttackMontageEnd = false;
			Boss2->CanMove = false;
		}));
	MontageEndMap.Add(Boss2AnimationType::DOUBLESMASH, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->CanMove = true;
			Boss2->IsLockOn = true;
		}));

	MontageStartMap.Add(Boss2AnimationType::SCREAMATTACK, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->IsAttackMontageEnd = false;
			Boss2->CanMove = false;
		}));
	MontageEndMap.Add(Boss2AnimationType::SCREAMATTACK, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->CanMove = true;
			Boss2->IsLockOn = true;
		}));

	MontageStartMap.Add(Boss2AnimationType::HEADATTACK, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->IsAttackMontageEnd = false;
			Boss2->CanMove = false;
		}));
	MontageEndMap.Add(Boss2AnimationType::HEADATTACK, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->CanMove = true;
			Boss2->IsLockOn = true;
		}));

	MontageStartMap.Add(Boss2AnimationType::CHARGE, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->IsAttackMontageEnd = false;
			Boss2->CanMove = false;
		}));
	MontageEndMap.Add(Boss2AnimationType::CHARGE, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->CanMove = true;
			Boss2->IsLockOn = true;
		}));

	MontageStartMap.Add(Boss2AnimationType::HEADING, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->AreaAtkPos->AttachToComponent(Boss2->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Bip001-Head"));
			Boss2->IsAttackMontageEnd = false;
			Boss2->CanMove = false;
		}));
	MontageEndMap.Add(Boss2AnimationType::HEADING, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->CanMove = true;
			Boss2->IsLockOn = true;
		}));

	MontageStartMap.Add(Boss2AnimationType::VOMITFALL, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->IsAttackMontageEnd = false;
			Boss2->CanMove = false;
		}));
	MontageEndMap.Add(Boss2AnimationType::VOMITFALL, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->CanMove = true;
			Boss2->IsLockOn = true;
		}));

	MontageStartMap.Add(Boss2AnimationType::ELBOWSPIN, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->IsAttackMontageEnd = false;
			Boss2->CanMove = false;
		}));
	MontageEndMap.Add(Boss2AnimationType::ELBOWSPIN, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->CanMove = true;
			Boss2->IsLockOn = true;
		}));

	MontageStartMap.Add(Boss2AnimationType::HUNTJUMP, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->IsAttackMontageEnd = false;
			Boss2->CanMove = false;
		}));
	MontageEndMap.Add(Boss2AnimationType::HUNTJUMP, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->CanMove = true;
			Boss2->IsLockOn = true;
		}));

	MontageStartMap.Add(Boss2AnimationType::JUMPEXPLOSION, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->GetWorldTimerManager().SetTimer(Boss2->JumpExplosionTimer, Boss2, &AJesusBoss2::JumpExplosionCheck, 1.f, true, 3.7f);
			Boss2->AreaAtkPos->AttachToComponent(Boss2->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Bip001-Neck"));
			Boss2->IsAttackMontageEnd = false;
			Boss2->CanMove = false;
		}));
	MontageEndMap.Add(Boss2AnimationType::JUMPEXPLOSION, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->CanMove = true;
			Boss2->IsLockOn = true;
		}));

	MontageStartMap.Add(Boss2AnimationType::THROWSTONE, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			auto RightHandSocket = Boss2->GetMesh()->GetSocketLocation(FName("RHand"));
			auto PoolObject = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[35].ObjClass,
				RightHandSocket, FRotator::ZeroRotator);
			Boss2->StonePoolObj = Cast<AStoneObjectInPool>(PoolObject);
			Boss2->StonePoolObj->SceneComp->AttachToComponent(Boss2->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RHand"));
			Boss2->IsAttackMontageEnd = false;
			Boss2->CanMove = false;
		}));
	MontageEndMap.Add(Boss2AnimationType::THROWSTONE, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->CanMove = true;
			Boss2->IsLockOn = true;
		}));

	//===========================================공격 실행=========================================================

	BossAttackMap.Add(Boss2ActionType::B2_FALLTHECROSS, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::CROSSFALL, Boss2, Boss2->MeleeActionArr, Boss2AttackType::B2_MELEE);
		}));
	//BossAttackMap.Add(Boss2ActionType::B2_FALLTHECROSS_BACK, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
	//	{
	//		Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::CROSSFALL, Boss2, Boss2->BackActionArr, Boss2AttackType::B2_BACKATK);
	//	}));
	//BossAttackMap.Add(Boss2ActionType::B2_FALLTHECROSS_LEFT, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
	//	{
	//		Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::CROSSFALL, Boss2, Boss2->LeftActionArr, Boss2AttackType::B2_LEFTATK);
	//	}));
	//BossAttackMap.Add(Boss2ActionType::B2_FALLTHECROSS_RIGHT, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
	//	{
	//		Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::CROSSFALL, Boss2, Boss2->RightActionArr, Boss2AttackType::B2_RIGHTATK);
	//	}));

	BossAttackMap.Add(Boss2ActionType::B2_SLASH, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::SLASH, Boss2, Boss2->MeleeActionArr, Boss2AttackType::B2_MELEE);
		}));

	BossAttackMap.Add(Boss2ActionType::B2_DOWNSMASH, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::DOWNSMASH, Boss2, Boss2->MeleeActionArr, Boss2AttackType::B2_MELEE);
		}));

	BossAttackMap.Add(Boss2ActionType::B2_DOUBLESMASH, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::DOUBLESMASH, Boss2, Boss2->MeleeActionArr, Boss2AttackType::B2_MELEE);
		}));

	BossAttackMap.Add(Boss2ActionType::B2_SCREAMATTACK, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::SCREAMATTACK, Boss2, Boss2->MeleeActionArr, Boss2AttackType::B2_MELEE);
		}));

	BossAttackMap.Add(Boss2ActionType::B2_HEADATTACK, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::HEADATTACK, Boss2, Boss2->FollowUpActionArr, Boss2AttackType::B2_FOLLOWUP);
		}));

	BossAttackMap.Add(Boss2ActionType::B2_CHARGE, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::CHARGE, Boss2, Boss2->RangeActionArr, Boss2AttackType::B2_RANGE);
		}));

	BossAttackMap.Add(Boss2ActionType::B2_HEADING, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::HEADING, Boss2, Boss2->MeleeActionArr, Boss2AttackType::B2_MELEE);
		}));
	//
	BossAttackMap.Add(Boss2ActionType::B2_VOMITFALL, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::VOMITFALL, Boss2, Boss2->MeleeActionArr, Boss2AttackType::B2_MELEE);
		}));

	//BossAttackMap.Add(Boss2ActionType::B2_ELBOWSPIN, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
	//	{
	//		Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::ELBOWSPIN, Boss2, Boss2->BackActionArr, Boss2AttackType::B2_BACKATK);
	//	}));

	BossAttackMap.Add(Boss2ActionType::B2_HUNTJUMP, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::HUNTJUMP, Boss2, Boss2->RangeActionArr, Boss2AttackType::B2_RANGE);
		}));

	BossAttackMap.Add(Boss2ActionType::B2_JUMPEXPLOSION, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::JUMPEXPLOSION, Boss2, Boss2->RangeActionArr, Boss2AttackType::B2_RANGE);
		}));

	BossAttackMap.Add(Boss2ActionType::B2_THROWSTONE, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::THROWSTONE, Boss2, Boss2->RangeActionArr, Boss2AttackType::B2_RANGE);
		}));

	//====================================공격타입에 맞는 공격이 끝났을 때 실행되는 맵====================================

	ActionEndMap.Add(Boss2AttackType::B2_MELEE, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{		
			Boss2ActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);
			auto name = Boss2AnimationEnum->GetNameStringByValue((uint64)Type);
			UE_LOG(LogTemp, Warning, TEXT("end attack name : %s"), *name);

			if (CurrentActionTemp.IsAddPercentage)
				InitPercentageMap[CurrentActionTemp.AttackType]();
			else
				ChangePercentageMap[CurrentActionTemp.AttackType](&CurrentActionTemp);

			if (Type == Boss2AnimationType::DOWNSMASH || Type == Boss2AnimationType::SLASH)
			{
				UE_LOG(LogTemp, Warning, TEXT("get follow up attack"));
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_FOLLOWUP]();
				SetBTAction(CurrentActionTemp);
				return;
			}

			SetBTAction(GetRandomPattern(Dist));

			IsAttackMontageEnd = true;
		}));
	ActionEndMap.Add(Boss2AttackType::B2_RANGE, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{
			Boss2ActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);

			if (CurrentActionTemp.IsAddPercentage)
				InitPercentageMap[CurrentActionTemp.AttackType]();
			else
				ChangePercentageMap[CurrentActionTemp.AttackType](&CurrentActionTemp);

			SetBTAction(GetRandomPattern(Dist));

			IsAttackMontageEnd = true;
		}));
	ActionEndMap.Add(Boss2AttackType::B2_FOLLOWUP, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{
			Boss2ActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);

			if (CurrentActionTemp.IsAddPercentage)
				InitPercentageMap[CurrentActionTemp.AttackType]();
			else
				ChangePercentageMap[CurrentActionTemp.AttackType](&CurrentActionTemp);

			SetBTAction(GetRandomPattern(Dist));
			IsAttackMontageEnd = true;
		}));

	ActionEndMap.Add(Boss2AttackType::B2_LEFTATK, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{
			Boss2ActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);
		
			if (CurrentActionTemp.IsAddPercentage)
				InitPercentageMap[CurrentActionTemp.AttackType]();
			else
				ChangePercentageMap[CurrentActionTemp.AttackType](&CurrentActionTemp);

			SetBTAction(GetRandomPattern(Dist));
			IsAttackMontageEnd = true;
		}));

	ActionEndMap.Add(Boss2AttackType::B2_RIGHTATK, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{
			Boss2ActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);
			
			if (CurrentActionTemp.IsAddPercentage)
				InitPercentageMap[CurrentActionTemp.AttackType]();
			else
				ChangePercentageMap[CurrentActionTemp.AttackType](&CurrentActionTemp);

			SetBTAction(GetRandomPattern(Dist));
			IsAttackMontageEnd = true;
		}));

	ActionEndMap.Add(Boss2AttackType::B2_BACKATK, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{
			Boss2ActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);
		
			if (CurrentActionTemp.IsAddPercentage)
				InitPercentageMap[CurrentActionTemp.AttackType]();
			else
				ChangePercentageMap[CurrentActionTemp.AttackType](&CurrentActionTemp);

			SetBTAction(GetRandomPattern(Dist));
			IsAttackMontageEnd = true;
		}));

	//======================================확률에 의한 랜덤 패턴 가져오기========================================

	GetRandomPatternMap.Add(Boss2AttackType::B2_MELEE, TFunction<Boss2ActionTemp()>([=]()
		{
			if (MeleeActionArr.Num() <= 0)
				return Boss2ActionTemp();

			auto RandNum = MakeRandom(MeleePercentageVec);

			//로그 찍기 용 패턴 이름
			auto PatternName = Boss2ActionEnum->GetNameStringByValue(MeleeActionArr[RandNum].ActionType);

			return  MeleeActionArr[RandNum];
		}));

	GetRandomPatternMap.Add(Boss2AttackType::B2_RANGE, TFunction<Boss2ActionTemp()>([=]()
		{
			if (RangeActionArr.Num() <= 0)
				return Boss2ActionTemp();

			auto RandNum = MakeRandom(RangePercentageVec);

			//로그 찍기 용 패턴 이름
			auto PatternName = Boss2ActionEnum->GetNameStringByValue(RangeActionArr[RandNum].ActionType);

			return  RangeActionArr[RandNum];
		}));

	GetRandomPatternMap.Add(Boss2AttackType::B2_FOLLOWUP, TFunction<Boss2ActionTemp()>([=]()
		{
			if (FollowUpActionArr.Num() <= 0)
				return Boss2ActionTemp();

			auto RandNum = MakeRandom(FollowUpPercentageVec);

			auto PatternName = Boss2ActionEnum->GetNameStringByValue(FollowUpActionArr[RandNum].ActionType);

			return  FollowUpActionArr[RandNum];
		}));

	//GetRandomPatternMap.Add(Boss2AttackType::B2_LEFTATK, TFunction<Boss2ActionTemp()>([=]()
	//	{
	//		if (LeftActionArr.Num() <= 0)
	//			return Boss2ActionTemp();

	//		auto RandNum = MakeRandom(LeftPercentageVec);

	//		//로그 찍기 용 패턴 이름
	//		auto PatternName = Boss2ActionEnum->GetNameStringByValue(LeftActionArr[RandNum].ActionType);

	//		return  LeftActionArr[RandNum];
	//	}));

	//GetRandomPatternMap.Add(Boss2AttackType::B2_RIGHTATK, TFunction<Boss2ActionTemp()>([=]()
	//	{
	//		if (RightActionArr.Num() <= 0)
	//			return Boss2ActionTemp();

	//		auto RandNum = MakeRandom(RightPercentageVec);

	//		//로그 찍기 용 패턴 이름
	//		auto PatternName = Boss2ActionEnum->GetNameStringByValue(RightActionArr[RandNum].ActionType);

	//		return  RightActionArr[RandNum];
	//	}));

	//GetRandomPatternMap.Add(Boss2AttackType::B2_BACKATK, TFunction<Boss2ActionTemp()>([=]()
	//	{
	//		if (BackActionArr.Num() <= 0)
	//			return Boss2ActionTemp();

	//		auto RandNum = MakeRandom(BackPercentageVec);

	//		//로그 찍기 용 패턴 이름
	//		auto PatternName = Boss2ActionEnum->GetNameStringByValue(BackActionArr[RandNum].ActionType);

	//		return  BackActionArr[RandNum];
	//	}));

	//===================================배열에 패턴 추가==============================================

	AddArrMap.Add(B2_MELEE, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
		{
			MeleeActionArr.Add(*Temp);
			MeleePercentageVec.push_back(Temp->Percentage);
		}));

	AddArrMap.Add(B2_RANGE, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
		{
			RangeActionArr.Add(*Temp);
			RangePercentageVec.push_back(Temp->Percentage);
		}));

	AddArrMap.Add(B2_FOLLOWUP, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
		{
			FollowUpActionArr.Add(*Temp);
			FollowUpPercentageVec.push_back(Temp->Percentage);
		}));

	//AddArrMap.Add(B2_LEFTATK, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
	//	{
	//		LeftActionArr.Add(*Temp);
	//		LeftPercentageVec.push_back(Temp->Percentage);
	//	}));

	//AddArrMap.Add(B2_RIGHTATK, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
	//	{
	//		RightActionArr.Add(*Temp);
	//		RightPercentageVec.push_back(Temp->Percentage);
	//	}));

	//AddArrMap.Add(B2_BACKATK, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
	//	{
	//		BackActionArr.Add(*Temp);
	//		BackPercentageVec.push_back(Temp->Percentage);
	//	}));

	//====================================확률 변경============================================

	ChangePercentageMap.Add(B2_MELEE, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
		{
			MeleePercentageVec.clear();

			for (int i = 0; i < MeleeActionArr.Num(); i++)
			{
				if (MeleeActionArr[i].ActionType == Temp->ActionType)
				{
					MeleeActionArr[i].Percentage -= DecreasePercentageVal;
					MeleePercentageVec.push_back(MeleeActionArr[i].Percentage);
					continue;
				}

				MeleeActionArr[i].IsAddPercentage = true;
				MeleeActionArr[i].Percentage += DecreasePercentageVal / (MeleeActionArr.Num() - 1);
				MeleePercentageVec.push_back(MeleeActionArr[i].Percentage);
			}
		}));

	ChangePercentageMap.Add(B2_RANGE, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
		{
			RangePercentageVec.clear();

			for (int i = 0; i < RangeActionArr.Num(); i++)
			{
				if (RangeActionArr[i].ActionType == Temp->ActionType)
				{
					RangeActionArr[i].Percentage -= DecreasePercentageVal;
					RangePercentageVec.push_back(RangeActionArr[i].Percentage);
					continue;
				}

				RangeActionArr[i].IsAddPercentage = true;
				RangeActionArr[i].Percentage += DecreasePercentageVal / (RangeActionArr.Num() - 1);
				RangePercentageVec.push_back(RangeActionArr[i].Percentage);
			}
		}));

	ChangePercentageMap.Add(B2_FOLLOWUP, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
		{
			FollowUpPercentageVec.clear();

			for (int i = 0; i < FollowUpActionArr.Num(); i++)
			{
				if (FollowUpActionArr[i].ActionType == Temp->ActionType)
				{
					FollowUpActionArr[i].Percentage -= DecreasePercentageVal;
					FollowUpPercentageVec.push_back(FollowUpActionArr[i].Percentage);
					continue;
				}

				FollowUpActionArr[i].IsAddPercentage = true;
				FollowUpActionArr[i].Percentage += DecreasePercentageVal / (FollowUpActionArr.Num() - 1);
				FollowUpPercentageVec.push_back(FollowUpActionArr[i].Percentage);
			}
		}));

	//ChangePercentageMap.Add(B2_LEFTATK, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
	//	{
	//		LeftPercentageVec.clear();

	//		for (int i = 0; i < LeftActionArr.Num(); i++)
	//		{
	//			if (LeftActionArr[i].ActionType == Temp->ActionType)
	//			{
	//				LeftActionArr[i].Percentage -= DecreasePercentageVal;
	//				LeftPercentageVec.push_back(LeftActionArr[i].Percentage);
	//				continue;
	//			}

	//			LeftActionArr[i].IsAddPercentage = true;
	//			LeftActionArr[i].Percentage += DecreasePercentageVal / (LeftActionArr.Num() - 1);
	//			LeftPercentageVec.push_back(LeftActionArr[i].Percentage);
	//		}
	//	}));

	//ChangePercentageMap.Add(B2_RIGHTATK, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
	//	{
	//		RightPercentageVec.clear();

	//		for (int i = 0; i < RightActionArr.Num(); i++)
	//		{
	//			if (RightActionArr[i].ActionType == Temp->ActionType)
	//			{
	//				RightActionArr[i].Percentage -= DecreasePercentageVal;
	//				RightPercentageVec.push_back(RightActionArr[i].Percentage);
	//				continue;
	//			}

	//			RightActionArr[i].IsAddPercentage = true;
	//			RightActionArr[i].Percentage += DecreasePercentageVal / (RightActionArr.Num() - 1);
	//			RightPercentageVec.push_back(RightActionArr[i].Percentage);
	//		}
	//	}));

	//ChangePercentageMap.Add(B2_BACKATK, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
	//	{
	//		BackPercentageVec.clear();

	//		for (int i = 0; i < BackActionArr.Num(); i++)
	//		{
	//			if (BackActionArr[i].ActionType == Temp->ActionType)
	//			{
	//				BackActionArr[i].Percentage -= DecreasePercentageVal;
	//				BackPercentageVec.push_back(BackActionArr[i].Percentage);
	//				continue;
	//			}

	//			BackActionArr[i].IsAddPercentage = true;
	//			BackActionArr[i].Percentage += DecreasePercentageVal / (BackActionArr.Num() - 1);
	//			BackPercentageVec.push_back(BackActionArr[i].Percentage);
	//		}
	//	}));

	//===============================확률 초기화========================================

	InitPercentageMap.Add(B2_MELEE, TFunction<void()>([=]()
		{
			MeleePercentageVec.clear();

			for (int i = 0; i < MeleeActionArr.Num(); i++)
			{
				auto BossActionName = Boss2ActionEnum->GetNameStringByValue(MeleeActionArr[i].ActionType);
				MeleeActionArr[i].Percentage = GetActionData(*BossActionName)->Percentage;
				MeleeActionArr[i].IsAddPercentage = false;
				MeleePercentageVec.push_back(MeleeActionArr[i].Percentage);
			}
		}));

	InitPercentageMap.Add(B2_RANGE, TFunction<void()>([=]()
		{
			RangePercentageVec.clear();

			for (int i = 0; i < RangeActionArr.Num(); i++)
			{
				auto BossActionName = Boss2ActionEnum->GetNameStringByValue(RangeActionArr[i].ActionType);
				RangeActionArr[i].Percentage = GetActionData(*BossActionName)->Percentage;
				RangeActionArr[i].IsAddPercentage = false;
				RangePercentageVec.push_back(RangeActionArr[i].Percentage);
			}
		}));

	InitPercentageMap.Add(B2_FOLLOWUP, TFunction<void()>([=]()
		{
			FollowUpPercentageVec.clear();

			for (int i = 0; i < FollowUpActionArr.Num(); i++)
			{
				auto BossActionName = Boss2ActionEnum->GetNameStringByValue(FollowUpActionArr[i].ActionType);
				FollowUpActionArr[i].Percentage = GetActionData(*BossActionName)->Percentage;
				FollowUpActionArr[i].IsAddPercentage = false;
				FollowUpPercentageVec.push_back(FollowUpActionArr[i].Percentage);
			}
		}));

	//InitPercentageMap.Add(B2_LEFTATK, TFunction<void()>([=]()
	//	{
	//		LeftPercentageVec.clear();

	//		for (int i = 0; i < LeftActionArr.Num(); i++)
	//		{
	//			auto BossActionName = Boss2ActionEnum->GetNameStringByValue(LeftActionArr[i].ActionType);
	//			LeftActionArr[i].Percentage = GetActionData(*BossActionName)->Percentage;
	//			LeftActionArr[i].IsAddPercentage = false;
	//			LeftPercentageVec.push_back(LeftActionArr[i].Percentage);
	//		}
	//	}));

	//InitPercentageMap.Add(B2_RIGHTATK, TFunction<void()>([=]()
	//	{
	//		RightPercentageVec.clear();

	//		for (int i = 0; i < RightActionArr.Num(); i++)
	//		{
	//			auto BossActionName = Boss2ActionEnum->GetNameStringByValue(RightActionArr[i].ActionType);
	//			RightActionArr[i].Percentage = GetActionData(*BossActionName)->Percentage;
	//			RightActionArr[i].IsAddPercentage = false;
	//			RightPercentageVec.push_back(RightActionArr[i].Percentage);
	//		}
	//	}));

	//InitPercentageMap.Add(B2_BACKATK, TFunction<void()>([=]()
	//	{
	//		BackPercentageVec.clear();

	//		for (int i = 0; i < RightActionArr.Num(); i++)
	//		{
	//			auto BossActionName = Boss2ActionEnum->GetNameStringByValue(BackActionArr[i].ActionType);
	//			BackActionArr[i].Percentage = GetActionData(*BossActionName)->Percentage;
	//			BackActionArr[i].IsAddPercentage = false;
	//			BackPercentageVec.push_back(BackActionArr[i].Percentage);
	//		}
	//	}));

	//=======================================피격 당한 부위에 따른 피격 모션=========================================

	BoneMap.Add(Boss2BoneRotateType::HEAD, TFunction<void()>([=]()
		{
			if (IsStartBoneRot)
			{
				Boss2AnimInstance->HeadBoneRotateVal = FMath::RInterpTo(Boss2AnimInstance->HeadBoneRotateVal, HeadBoneRotVal, DeltaSeconds, Speed);
			}
			else
			{
				Boss2AnimInstance->HeadBoneRotateVal = FMath::RInterpTo(Boss2AnimInstance->HeadBoneRotateVal,
					Boss2AnimInstance->CurrentBoneRotateVal, ReturnDeltaSeconds, ReturnSpeed);
				GetWorldTimerManager().ClearTimer(BoneRotateTimerHandle);
			}
		}));

	BoneMap.Add(Boss2BoneRotateType::LEFTARM, TFunction<void()>([=]()
		{
			if (IsStartBoneRot)
			{
				Boss2AnimInstance->LArmBoneRotateVal = FMath::RInterpTo(Boss2AnimInstance->LArmBoneRotateVal, LArmBoneRotVal, DeltaSeconds, Speed);
			}
			else
			{
				Boss2AnimInstance->LArmBoneRotateVal = FMath::RInterpTo(Boss2AnimInstance->LArmBoneRotateVal,
					Boss2AnimInstance->CurrentBoneRotateVal, ReturnDeltaSeconds, ReturnSpeed);
				GetWorldTimerManager().ClearTimer(BoneRotateTimerHandle);
			}
		}));

	BoneMap.Add(Boss2BoneRotateType::RIGHTARM, TFunction<void()>([=]()
		{
			if (IsStartBoneRot)
			{
				Boss2AnimInstance->RArmBoneRotateVal = FMath::RInterpTo(Boss2AnimInstance->RArmBoneRotateVal, RArmBoneRotVal, DeltaSeconds, Speed);
			}
			else
			{
				Boss2AnimInstance->RArmBoneRotateVal = FMath::RInterpTo(Boss2AnimInstance->RArmBoneRotateVal,
					Boss2AnimInstance->CurrentBoneRotateVal, ReturnDeltaSeconds, ReturnSpeed);
				GetWorldTimerManager().ClearTimer(BoneRotateTimerHandle);
			}
		}));

	//=======================================보스 공격 콜리전 부위별로 켜줌=========================================

	CollisionMap.Add(Boss2CollisionType::HEAD, TFunction<void(bool)>([=](bool OnOff)
		{
			if (OnOff)
				HeadAtkCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			else
			{
				HeadAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Damage = 0;
			}
		}));
	CollisionMap.Add(Boss2CollisionType::LEFTARM, TFunction<void(bool)>([=](bool OnOff)
		{
			if (OnOff)
				LeftAtkCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			else
			{
				LeftAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Damage = 0;
			}
		}));
	CollisionMap.Add(Boss2CollisionType::RIGHTARM, TFunction<void(bool)>([=](bool OnOff)
		{
			if (OnOff)
				RightAtkCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			else
			{
				RightAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Damage = 0;
			}
		}));
	CollisionMap.Add(Boss2CollisionType::CHARGE, TFunction<void(bool)>([=](bool OnOff)
		{
			if (OnOff)
				ChargeAtkCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			else
			{
				ChargeAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Damage = 0;
			}
		}));
}

AJesusBoss2::~AJesusBoss2()
{
	MeleePercentageVec.clear();
	MeleePercentageVec.shrink_to_fit();
	
	RangePercentageVec.clear();
	RangePercentageVec.shrink_to_fit();

	FollowUpPercentageVec.clear();
	FollowUpPercentageVec.shrink_to_fit();

	//LeftPercentageVec.clear();
	//LeftPercentageVec.shrink_to_fit();
	//
	//RightPercentageVec.clear();
	//RightPercentageVec.shrink_to_fit();
	//
	//BackPercentageVec.clear();
	//BackPercentageVec.shrink_to_fit();
}

void AJesusBoss2::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Boss2AnimInstance = Cast<UBoss2AnimInstance>(GetMesh()->GetAnimInstance());

	if (Boss2AnimInstance)
	{
		Boss2AnimInstance->OnCrossFall.AddUObject(this, &AJesusBoss2::OnCrossFall);
		Boss2AnimInstance->OnVomitFall.AddUObject(this, &AJesusBoss2::OnVomitFall);
		Boss2AnimInstance->OnStart.AddUObject(this, &AJesusBoss2::OnStart);
		Boss2AnimInstance->OnEnd.AddUObject(this, &AJesusBoss2::OnEnd);
		Boss2AnimInstance->OnLockOn.AddUObject(this, &AJesusBoss2::LockOn);
		Boss2AnimInstance->OnLockOff.AddUObject(this, &AJesusBoss2::LockOff);
		Boss2AnimInstance->OnJumpStart.AddUObject(this, &AJesusBoss2::SlerpJump);
		Boss2AnimInstance->OnJumpEnd.AddUObject(this, &AJesusBoss2::SlerpJumpEnd);
		Boss2AnimInstance->OnStoneThrow.AddUObject(this, &AJesusBoss2::ThrowStone);

		Boss2AnimInstance->OnMontageEnded.AddDynamic(this, &AJesusBoss2::GetEndedMontage);
	}
}

void AJesusBoss2::BeginPlay()
{
	Super::BeginPlay();

	SetMetaData();
	AIController = Cast<ABoss2AIController>(GetController());
	Boss2ActionEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("Boss2ActionType"), true);
	Boss2AnimationEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("Boss2AnimationType"), true);
	GetCharacterMovement()->MaxWalkSpeed = BossDataStruct.CharacterOriginSpeed;
	MonsterLockOnWidget = Cast<UMonsterWidget>(LockOnWidget->GetWidget());
	MonsterLockOnWidget->LockOnImage->SetVisibility(ESlateVisibility::Hidden);
	
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	LeftAtkCollision->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss2::AttackHit);
	LeftAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightAtkCollision->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss2::AttackHit);
	RightAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadAtkCollision->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss2::AttackHit);
	HeadAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChargeAtkCollision->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss2::AttackHit);
	ChargeAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	JumpExplosonCollider->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss2::AttackHit);
	JumpExplosonCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss2::SetBoneHead);
	HeadHitCollision->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss2::SetBoneHead);
	LeftArmHitCollision->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss2::SetBoneLArm);
	RightArmHitCollision->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss2::SetBoneRArm);
	
	//임시로 변수 설정
	CanMove = true;
	IsLockOn = true;
	Boss2AnimInstance->IsStart = true;
}

void AJesusBoss2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter->TargetComp != nullptr)
	{
		if (PlayerCharacter->TargetComp->GetOwner() == this)
		{
			MonsterLockOnWidget->LockOnImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			MonsterLockOnWidget->LockOnImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (JumpExplosionCnt >= 3)
	{
		GetWorldTimerManager().ClearTimer(JumpExplosionTimer);
		JumpExplosionCnt = 0;
		JumpExplosonCollider->SetSphereRadius(1.f);
		JumpExplosonCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (AttackLockOn)
		RotateToPlayerInterp();

	if (JumpMoveStart)
		JumpMove();

	PlayMoveMontage();

	IsGameStart = Boss2AnimInstance->IsStart;

	//본 회전시키는 코드
	BoneMap[Boss2AnimInstance->CurrentBoneType]();
}

/*=====================
*		Function
=====================*/

void AJesusBoss2::PlayMoveMontage()
{
	if (Boss2AnimInstance->Speed > 10 && !IsAttacking && IsPlayerAlive
		&& AIController->GetBlackboardComponent()->GetValueAsBool("CanMove"))
	{
		if (CurrentAnimType == Boss2AnimationType::DIE)
			return;

		if (!IsMontagePlay)
		{
			ChangeMontageAnimation(Boss2AnimationType::FOWARDWALK);
			IsMontagePlay = true;
		}
	}
	else
	{
		if (!Boss2AnimInstance->IsAnyMontagePlaying())
		{
			ChangeMontageAnimation(Boss2AnimationType::IDLE);
			IsMontagePlay = false;
		}
	}
}

void AJesusBoss2::RotateToPlayerInterp()
{
	FRotator ToTarget = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerCharacter->GetActorLocation());
	FRotator LookAtRotation = FMath::RInterpTo(GetActorRotation(), ToTarget, GetWorld()->DeltaTimeSeconds, 4.f);
	SetActorRotation(LookAtRotation);
}

void AJesusBoss2::SetMetaData()
{
	Boss2AnimationType Type;
	UAnimMontage* Montage;

	for (auto iter = Boss2MontageMap.begin(); iter != Boss2MontageMap.end(); iter.operator++())
	{
		Type = iter.Key();
		Montage = iter.Value();

		Montage->EmptyMetaData();
		UBoss2AnimMetaData* MetaData = NewObject<UBoss2AnimMetaData>();
		MetaData->AnimType = Type;
		Montage->AddMetaData(MetaData);
	}
}

Boss2AnimationType AJesusBoss2::GetTypeFromMetaData(UAnimMontage* Montage)
{
	auto MetaDataArr = Montage->GetMetaData();

	if (MetaDataArr.Num() == 0)
		return Boss2AnimationType::NONE;

	return Cast<UBoss2AnimMetaData>(MetaDataArr[0])->AnimType;
}

void AJesusBoss2::ChangeMontageAnimation(Boss2AnimationType Type)
{
	Boss2AnimInstance->PlayMontage(Boss2MontageMap[Type]);
	ChangeAnimType(Type);
}

void AJesusBoss2::ChangeAnimType(Boss2AnimationType Type)
{
	CurrentAnimType = Type;
}

FBoss2Action* AJesusBoss2::GetActionData(FName Name)
{
	return	Boss2Actions->FindRow<FBoss2Action>(Name, TEXT(""));
}

void AJesusBoss2::DoTypeAttack(float MinRange, float MaxRange, float Dist, bool LockOn, Boss2AnimationType Type, AJesusBoss2* Boss2, TArray<Boss2ActionTemp> &arr, Boss2AttackType AtkType)
{
 	if (Dist >= MinRange && Dist <= MaxRange && !Boss2->CurrentActionTemp.IsExcute)
	{
		//InitIsExcute();

		//스레드 사용
		JesusThreadManager& t = JesusThreadManager::GetInstance();

		t.EnqueueJob(TFunction<void(void)>([=](void)
			{ 				
				std::unique_lock<std::mutex> lock(m1);
				for (int i = 0; i < MeleeActionArr.Num(); i++)
					MeleeActionArr[i].IsExcute = false;
				for (int i = 0; i < RangeActionArr.Num(); i++)
					RangeActionArr[i].IsExcute = false;
				for (int i = 0; i < FollowUpActionArr.Num(); i++)
					FollowUpActionArr[i].IsExcute = false;
			}));

		if (!Boss2->CurrentActionTemp.CanContinuity)
		{
			auto FoundIndex = arr.Find(Boss2->CurrentActionTemp);
			if (FoundIndex == INDEX_NONE)
			{
				CurrentActionTemp = GetRandomPatternMap[AtkType]();
				SetBTAction(CurrentActionTemp);
				return;
			}
			
			std::unique_lock<std::mutex> lock(m1);
			arr[FoundIndex].IsExcute = true;
		}

		Boss2->CanMove = false;
		Boss2->GetCharacterMovement()->MaxWalkSpeed = CurrentActionTemp.Speed;
		Boss2->PlayAttackAnim(Type);
		Boss2->IsLockOn = LockOn;
		Boss2->IsMontagePlay = false;
	}
	else
	{
		CurrentActionTemp = GetRandomPatternMap[AtkType]();
		SetBTAction(CurrentActionTemp);
	}
}

void AJesusBoss2::InitIsExcute()
{
	for (int i = 0; i < MeleeActionArr.Num(); i++)
		MeleeActionArr[i].IsExcute = false;
	for (int i = 0; i < RangeActionArr.Num(); i++)
		RangeActionArr[i].IsExcute = false;
	for (int i = 0; i < FollowUpActionArr.Num(); i++)
		FollowUpActionArr[i].IsExcute = false;
	//for (int i = 0; i < LeftActionArr.Num(); i++)
	//	LeftActionArr[i].IsExcute = false;
	//for (int i = 0; i < RightActionArr.Num(); i++)
	//	RightActionArr[i].IsExcute = false;
	//for (int i = 0; i < BackActionArr.Num(); i++)
	//	BackActionArr[i].IsExcute = false;
}

void AJesusBoss2::SetBTAction(Boss2ActionTemp Temp)
{
	AIController->GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Boss2ActionType")), Temp.ActionType);
}

void AJesusBoss2::PlayAttackAnim(Boss2AnimationType Type)
{
	if (IsAttacking)
		return;

	IsAttacking = true;

	Boss2AnimInstance->StopAllMontages(0.25f);
	ChangeMontageAnimation(Type);
}

void AJesusBoss2::AttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FDamageEvent DamageEvent;

	auto Player = Cast<APlayerCharacter>(OtherActor);
	if (Player == nullptr)
		return;

	HeadAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChargeAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	auto Type = GetTypeFromMetaData(StartMontage);

	if (!Player->Imotal)
	{
		if (BossDataStruct.DamageList.Contains(Type))
			Damage += BossDataStruct.DamageList[Type];
		else
			return;
		
		OtherActor->TakeDamage(Damage, DamageEvent, GetController(), this);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[8].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[9].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[31].ObjClass, OtherActor->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
	}
}

float AJesusBoss2::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	OffHitCollision();

	BossDataStruct.CharacterHp -= DamageAmount;

	IsStartBoneRot = true;
	GetWorldTimerManager().SetTimer(BoneRotateTimerHandle, this, &AJesusBoss2::ReSetBoneRot, Time, false);
		
	//TODO : 보스 체력 UI

	//TODO : 그로기 관련 코드

	return DamageAmount;
}

void AJesusBoss2::HitStop()
{
	Super::HitStop();

	if (Boss2AnimInstance->GetCurrentActiveMontage() != nullptr)
		Boss2AnimInstance->PauseAnimation(Boss2AnimInstance->GetCurrentActiveMontage());
}

void AJesusBoss2::ResumeMontage()
{
	if (Boss2AnimInstance->GetCurrentActiveMontage() != nullptr)
		Boss2AnimInstance->ResumeMontage(Boss2AnimInstance->GetCurrentActiveMontage());
}

void AJesusBoss2::RespawnCharacter()
{
	Super::RespawnCharacter();
	SpawnInit();
}

void AJesusBoss2::PlayExecutionAnimation()
{
}

void AJesusBoss2::ActivateLockOnImage(bool value)
{
	value ? MonsterLockOnWidget->SetVisibility(ESlateVisibility::HitTestInvisible) : MonsterLockOnWidget->SetVisibility(ESlateVisibility::Collapsed);
}

FVector AJesusBoss2::Lerp(const FVector& start, const FVector& end, const float t)
{
	FVector NewLoc = FMath::Lerp(start, end, t);
	return NewLoc;
}

Boss2ActionTemp AJesusBoss2::GetRandomPattern(float Dist)
{
	if (Dist >= RangeAtk)
		CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_RANGE]();
	else
	{
		CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_MELEE]();

		//switch (PlayerDirection)
		//{
		//case B2_FOWARD:
		//	CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_MELEE]();
		//	break;
		//case B2_BACK:
		//	CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_BACKATK]();
		//	break;
		//case B2_LEFT:
		//	CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_LEFTATK]();
		//	break;
		//case B2_RIGHT:
		//	CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_RIGHTATK]();
		//	break;

		//default:
		//	UE_LOG(LogTemp, Warning, TEXT("PlayerDirection is not set!"));
		//	break;
		//}
	}

	return CurrentActionTemp;
}

void AJesusBoss2::SpawnInit()
{
	BossDataStruct.CharacterHp = BossDataStruct.CharacterMaxHp;
	BossDataStruct.CharacterOriginSpeed = 60.f;
	//TODO : UI 초기화
	IsDead = false;

	//패턴 확률 초기화
	InitPercentageMap[Boss2AttackType::B2_MELEE]();
	InitPercentageMap[Boss2AttackType::B2_RANGE]();
	InitPercentageMap[Boss2AttackType::B2_FOLLOWUP]();
	InitPercentageMap[Boss2AttackType::B2_LEFTATK]();
	InitPercentageMap[Boss2AttackType::B2_RIGHTATK]();
	InitPercentageMap[Boss2AttackType::B2_BACKATK]();

	//애니
	ChangeMontageAnimation(Boss2AnimationType::IDLE);

	//BT
	AIController->GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Boss2BaseAction")), B2_SUPER_MOVE);
	CurrentActionTemp = MeleeActionArr[0];
	AIController->GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Boss2ActionType")), CurrentActionTemp.ActionType);
}

void AJesusBoss2::StartBoneRot()
{
	Boss2AnimInstance->HeadBoneRotateVal = FMath::RInterpTo(Boss2AnimInstance->HeadBoneRotateVal, HeadBoneRotVal, DeltaSeconds, Speed);
}

void AJesusBoss2::ReSetBoneRot()
{
	IsStartBoneRot = false;
}

void AJesusBoss2::OffHitCollision()
{
	HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadHitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftArmHitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightArmHitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AJesusBoss2::SlerpJump()
{
	GetCapsuleComponent()->SetCollisionProfileName("IgnorePlayer");
	JumpMoveStart = true;
}

void AJesusBoss2::SlerpJumpEnd()
{
	JumpMoveStart = false;
	GetCapsuleComponent()->SetCollisionProfileName("AIPhysics");
}

void AJesusBoss2::JumpMove()
{
	auto NewLoc = Lerp(GetActorLocation(), LastPlayerLoc, 0.05f);
	SetActorLocation(NewLoc);
}

void AJesusBoss2::JumpExplosionCheck()
{
	if(JumpExplosionCnt == 0)
		JumpExplosonCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	JumpExplosionCnt += 1;

	if (JumpExplosionCnt == 1)
		JumpExplosonCollider->SetSphereRadius(400.f);
	else if (JumpExplosionCnt == 2)
		JumpExplosonCollider->SetSphereRadius(700.f);
	else if (JumpExplosionCnt == 3)
		JumpExplosonCollider->SetSphereRadius(1000.f);
}

void AJesusBoss2::SetBoneHead(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Boss2AnimInstance->CurrentBoneType = Boss2BoneRotateType::HEAD;
}

void AJesusBoss2::SetBoneLArm(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Boss2AnimInstance->CurrentBoneType = Boss2BoneRotateType::LEFTARM;
}

void AJesusBoss2::SetBoneRArm(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Boss2AnimInstance->CurrentBoneType = Boss2BoneRotateType::RIGHTARM;
}

void AJesusBoss2::ActivateHitCollision()
{
	HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HeadHitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftArmHitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightArmHitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void AJesusBoss2::GetEndedMontage(UAnimMontage* Montage, bool bInterrupted)
{
	IsStart.Exchange(false);
	auto Type = GetTypeFromMetaData(Montage);
}

/*=====================
		Notify
=====================*/
void AJesusBoss2::OnCrossFall()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (NavSystem == nullptr)
		return;

	GetWorld()->GetTimerManager().SetTimer(CrossSpawnTimerHandle, FTimerDelegate::CreateLambda([=]()
		{			
			if (CrossQueue.IsEmpty())
			{
				GetWorld()->GetTimerManager().ClearTimer(CrossSpawnTimerHandle);
				return;
			}
			else
			{
				ABaseObjectInPool* TempObj;
				CrossQueue.Dequeue(TempObj);
				TempObj->SetActorTickEnabled(true);
			}
		}), DelayBetweenCross, true, SpawnTime);

	GetWorld()->GetTimerManager().SetTimer(CrossTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			FNavLocation RandomLocation;

			std::random_device rd;
			std::mt19937 gen(rd());

			std::uniform_int_distribution<> RandomRotX(X_MinMax.X, X_MinMax.Y);
			std::uniform_int_distribution<> RandomRotY(Y_MinMax.X, Y_MinMax.Y);
			std::uniform_int_distribution<> RandomRotZ(Z_MinMax.X, Z_MinMax.Y);
			
			float ScaleTempX = RandomRotX(gen);
			float ScaleTempY = RandomRotY(gen);
			float ScaleTempZ = RandomRotZ(gen);

			if (NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), CrossMaxRange, RandomLocation))
			{
				auto PoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[32].ObjClass,
					RandomLocation.Location + FVector(0,0,SpawnHeight), FRotator(ScaleTempY, ScaleTempZ, ScaleTempX));
				CrossQueue.Enqueue(PoolObj);
				auto CastObj = Cast<AActor>(PoolObj);
				CastObj->SetActorScale3D(FVector(5.f, 5.f, 5.f));
				CurrentCrossCount++;
			}

			if (CurrentCrossCount >= CrossCount)
			{
				CurrentCrossCount = 0;
				GetWorld()->GetTimerManager().ClearTimer(CrossTimerHandle);
				return;
			}

		}), SpawnDelay, true);
}

void AJesusBoss2::OnVomitFall()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (NavSystem == nullptr)
		return;

	TArray<AVomitObjectInPool*> VomitArr;

	for (int i = 0; i < VomitCount; i++)
	{
		auto PoolObject = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[34].ObjClass,
			HeadAtkCollision->GetComponentLocation(),FRotator::ZeroRotator);
		auto CastObject = Cast<AVomitObjectInPool>(PoolObject);
		CastObject->SphereCollision->AddImpulse(FVector(0, 0, 1200));
		CastObject->ProjectileEffect->Activate();
		VomitArr.Add(CastObject);
	}

	GetWorld()->GetTimerManager().SetTimer(VomitTimerHandle, FTimerDelegate::CreateLambda([=]()
		{	
			FNavLocation RandomLocation;

			for (int i = 0; i < VomitArr.Num(); i++)
			{
				if (NavSystem->GetRandomPointInNavigableRadius(PlayerCharacter->GetActorLocation(), VomitMaxRange, RandomLocation))
				{
					VomitArr[i]->arcValue = 0.5f;
					VomitArr[i]->ShootProjectile(RandomLocation.Location);
				}

				if(i == VomitArr.Num()-1)
					GetWorld()->GetTimerManager().ClearTimer(VomitTimerHandle);
			}
		}), delay, false);
}

void AJesusBoss2::OnStart()
{
	if (!IsStart.Exchange(true))
	{
		StartEnd.Key = true;
		StartEnd.Value = false;

		StartMontage = GetCurrentMontage();
		//UE_LOG(LogTemp, Warning, TEXT("Start : %s"), *StartMontage->GetName());
		MontageStartMap[GetTypeFromMetaData(StartMontage)](this);
	}
}

void AJesusBoss2::OnEnd()
{
	StartEnd.Key = false;
	StartEnd.Value = true;

	GetCapsuleComponent()->SetCollisionProfileName("AIPhysics");
	IsAttacking = false;
	AttackLockOn = false;

	auto Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	float Dist = 0.0f;
	if (Player)
		Dist = FVector::Dist(GetActorLocation(), Player->GetActorLocation());

	//UE_LOG(LogTemp, Warning, TEXT("End : %s"), *StartMontage->GetName());

	EndedMontage = StartMontage;

	auto Type = GetTypeFromMetaData(EndedMontage);

	if (!IsEnd.Exchange(true))
		MontageEndMap[Type](this);

	if (!IsDead)
		ActionEndMap[CurrentActionTemp.AttackType](Dist, fDeltaTime, StartMontage);

	IsEnd.Exchange(false);
	IsStart.Exchange(false);
	IsAttackMontageEnd = true;
	IsMontagePlay = false;
}

void AJesusBoss2::LockOn()
{
	AttackLockOn = true;
}

void AJesusBoss2::LockOff()
{
	AttackLockOn = false;
	LastPlayerLoc = PlayerCharacter->GetActorLocation();
}

void AJesusBoss2::ThrowStone()
{
	if (StonePoolObj != nullptr)
	{
		StonePoolObj->SceneComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		StonePoolObj->SetActorTickEnabled(true);
	}
}

void AJesusBoss2::IsNotifyActive(bool value)
{
}

