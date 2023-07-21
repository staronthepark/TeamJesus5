// Fill out your copyright notice in the Description page of Project Settings.


#include "JesusBoss2.h"
#include "..\Player\PlayerCharacter.h"
#include <random>
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "..\Manager\CombatManager.h"
#include "DrawDebugHelpers.h"

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

	AreaAtkPos = CreateDefaultSubobject<USceneComponent>(TEXT("AreaAtkPos"));
	AreaAtkPos->SetupAttachment(GetMesh());
	AreaAtkPos->SetupAttachment(GetMesh(), FName("RHand"));


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


	//===========================================공격 실행=========================================================

	BossAttackMap.Add(Boss2ActionType::B2_FALLTHECROSS, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::CROSSFALL, Boss2, Boss2->MeleeActionArr, Boss2AttackType::B2_MELEE);
		}));
	BossAttackMap.Add(Boss2ActionType::B2_FALLTHECROSS_BACK, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::CROSSFALL, Boss2, Boss2->BackActionArr, Boss2AttackType::B2_BACKATK);
		}));
	BossAttackMap.Add(Boss2ActionType::B2_FALLTHECROSS_LEFT, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::CROSSFALL, Boss2, Boss2->LeftActionArr, Boss2AttackType::B2_LEFTATK);
		}));
	BossAttackMap.Add(Boss2ActionType::B2_FALLTHECROSS_RIGHT, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoTypeAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, false, Boss2AnimationType::CROSSFALL, Boss2, Boss2->RightActionArr, Boss2AttackType::B2_RIGHTATK);
		}));

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

	//====================================공격타입에 맞는 공격이 끝났을 때 실행되는 맵====================================

	ActionEndMap.Add(Boss2AttackType::B2_MELEE, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{		
			Boss2ActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);
		
			if (CurrentActionTemp.IsAddPercentage)
				InitPercentageMap[CurrentActionTemp.AttackType]();
			else
				ChangePercentageMap[CurrentActionTemp.AttackType](&CurrentActionTemp);

			if (Type == Boss2AnimationType::DOWNSMASH || Type == Boss2AnimationType::SLASH)
			{
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_FOLLOWUP]();
				SetBTAction(CurrentActionTemp);
				IsAttackMontageEnd = true;
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

	GetRandomPatternMap.Add(Boss2AttackType::B2_LEFTATK, TFunction<Boss2ActionTemp()>([=]()
		{
			if (LeftActionArr.Num() <= 0)
				return Boss2ActionTemp();

			auto RandNum = MakeRandom(LeftPercentageVec);

			//로그 찍기 용 패턴 이름
			auto PatternName = Boss2ActionEnum->GetNameStringByValue(LeftActionArr[RandNum].ActionType);

			return  LeftActionArr[RandNum];
		}));

	GetRandomPatternMap.Add(Boss2AttackType::B2_RIGHTATK, TFunction<Boss2ActionTemp()>([=]()
		{
			if (RightActionArr.Num() <= 0)
				return Boss2ActionTemp();

			auto RandNum = MakeRandom(RightPercentageVec);

			//로그 찍기 용 패턴 이름
			auto PatternName = Boss2ActionEnum->GetNameStringByValue(RightActionArr[RandNum].ActionType);

			return  RightActionArr[RandNum];
		}));

	GetRandomPatternMap.Add(Boss2AttackType::B2_BACKATK, TFunction<Boss2ActionTemp()>([=]()
		{
			if (BackActionArr.Num() <= 0)
				return Boss2ActionTemp();

			auto RandNum = MakeRandom(BackPercentageVec);

			//로그 찍기 용 패턴 이름
			auto PatternName = Boss2ActionEnum->GetNameStringByValue(BackActionArr[RandNum].ActionType);

			return  BackActionArr[RandNum];
		}));


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

	AddArrMap.Add(B2_LEFTATK, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
		{
			LeftActionArr.Add(*Temp);
			LeftPercentageVec.push_back(Temp->Percentage);
		}));

	AddArrMap.Add(B2_RIGHTATK, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
		{
			RightActionArr.Add(*Temp);
			RightPercentageVec.push_back(Temp->Percentage);
		}));

	AddArrMap.Add(B2_BACKATK, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
		{
			BackActionArr.Add(*Temp);
			BackPercentageVec.push_back(Temp->Percentage);
		}));

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
				MeleeActionArr[i].Percentage += DecreasePercentageVal / MeleeActionArr.Num() - 1;
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
				RangeActionArr[i].Percentage += DecreasePercentageVal / RangeActionArr.Num() - 1;
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
				FollowUpActionArr[i].Percentage += DecreasePercentageVal / FollowUpActionArr.Num() - 2;
				FollowUpPercentageVec.push_back(FollowUpActionArr[i].Percentage);
			}
		}));

	ChangePercentageMap.Add(B2_LEFTATK, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
		{
			LeftPercentageVec.clear();

			for (int i = 0; i < LeftActionArr.Num(); i++)
			{
				if (LeftActionArr[i].ActionType == Temp->ActionType)
				{
					LeftActionArr[i].Percentage -= DecreasePercentageVal;
					LeftPercentageVec.push_back(LeftActionArr[i].Percentage);
					continue;
				}

				LeftActionArr[i].IsAddPercentage = true;
				LeftActionArr[i].Percentage += DecreasePercentageVal / LeftActionArr.Num() - 2;
				LeftPercentageVec.push_back(LeftActionArr[i].Percentage);
			}
		}));

	ChangePercentageMap.Add(B2_RIGHTATK, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
		{
			RightPercentageVec.clear();

			for (int i = 0; i < RightActionArr.Num(); i++)
			{
				if (RightActionArr[i].ActionType == Temp->ActionType)
				{
					RightActionArr[i].Percentage -= DecreasePercentageVal;
					RightPercentageVec.push_back(RightActionArr[i].Percentage);
					continue;
				}

				RightActionArr[i].IsAddPercentage = true;
				RightActionArr[i].Percentage += DecreasePercentageVal / RightActionArr.Num() - 1;
				RightPercentageVec.push_back(RightActionArr[i].Percentage);
			}
		}));

	ChangePercentageMap.Add(B2_BACKATK, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
		{
			BackPercentageVec.clear();

			for (int i = 0; i < BackActionArr.Num(); i++)
			{
				if (BackActionArr[i].ActionType == Temp->ActionType)
				{
					BackActionArr[i].Percentage -= DecreasePercentageVal;
					BackPercentageVec.push_back(BackActionArr[i].Percentage);
					continue;
				}

				BackActionArr[i].IsAddPercentage = true;
				BackActionArr[i].Percentage += DecreasePercentageVal / BackActionArr.Num() - 1;
				BackPercentageVec.push_back(BackActionArr[i].Percentage);
			}
		}));

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

	InitPercentageMap.Add(B2_LEFTATK, TFunction<void()>([=]()
		{
			LeftPercentageVec.clear();

			for (int i = 0; i < LeftActionArr.Num(); i++)
			{
				auto BossActionName = Boss2ActionEnum->GetNameStringByValue(LeftActionArr[i].ActionType);
				LeftActionArr[i].Percentage = GetActionData(*BossActionName)->Percentage;
				LeftActionArr[i].IsAddPercentage = false;
				LeftPercentageVec.push_back(LeftActionArr[i].Percentage);
			}
		}));

	InitPercentageMap.Add(B2_RIGHTATK, TFunction<void()>([=]()
		{
			RightPercentageVec.clear();

			for (int i = 0; i < RightActionArr.Num(); i++)
			{
				auto BossActionName = Boss2ActionEnum->GetNameStringByValue(RightActionArr[i].ActionType);
				RightActionArr[i].Percentage = GetActionData(*BossActionName)->Percentage;
				RightActionArr[i].IsAddPercentage = false;
				RightPercentageVec.push_back(RightActionArr[i].Percentage);
			}
		}));

	InitPercentageMap.Add(B2_BACKATK, TFunction<void()>([=]()
		{
			BackPercentageVec.clear();

			for (int i = 0; i < RightActionArr.Num(); i++)
			{
				auto BossActionName = Boss2ActionEnum->GetNameStringByValue(BackActionArr[i].ActionType);
				BackActionArr[i].Percentage = GetActionData(*BossActionName)->Percentage;
				BackActionArr[i].IsAddPercentage = false;
				BackPercentageVec.push_back(BackActionArr[i].Percentage);
			}
		}));

	//=======================================플레이어 공격타입에 따른 피격 모션=========================================

	HitMap.Add(ActionType::ATTACK, TFunction<void()>([=]()
		{
			if (HitCount >= 2)
				return;

			HitCount++;

			//ChangeMontageAnimation(BossAnimationType::HIT);
		}));

	HitMap.Add(ActionType::POWERATTACK, TFunction<void()>([=]()
		{
			if (HitCount >= 2)
				return;

			HitCount++;

			//ChangeMontageAnimation(BossAnimationType::HIT);
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

	LeftPercentageVec.clear();
	LeftPercentageVec.shrink_to_fit();
	
	RightPercentageVec.clear();
	RightPercentageVec.shrink_to_fit();
	
	BackPercentageVec.clear();
	BackPercentageVec.shrink_to_fit();
}

void AJesusBoss2::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Boss2AnimInstance = Cast<UBoss2AnimInstance>(GetMesh()->GetAnimInstance());

	if (Boss2AnimInstance)
	{
		Boss2AnimInstance->OnCrossFall.AddUObject(this, &AJesusBoss2::OnCrossFall);
		Boss2AnimInstance->OnStart.AddUObject(this, &AJesusBoss2::OnStart);
		Boss2AnimInstance->OnEnd.AddUObject(this, &AJesusBoss2::OnEnd);
		Boss2AnimInstance->OnRightEnable.AddUObject(this, &AJesusBoss2::RightCollisionEnableNotify);
		Boss2AnimInstance->OnRightDisable.AddUObject(this, &AJesusBoss2::RightCollisionDisableNotify);
		Boss2AnimInstance->OnLeftEnable.AddUObject(this, &AJesusBoss2::LeftCollisionEnableNotify);
		Boss2AnimInstance->OnLeftDisable.AddUObject(this, &AJesusBoss2::LeftCollisionDisableNotify);
		Boss2AnimInstance->OnHeadEnable.AddUObject(this, &AJesusBoss2::HeadCollisionEnableNotify);
		Boss2AnimInstance->OnHeadDisable.AddUObject(this, &AJesusBoss2::HeadCollisionDisableNotify);
		Boss2AnimInstance->OnLockOn.AddUObject(this, &AJesusBoss2::LockOn);
		Boss2AnimInstance->OnLockOff.AddUObject(this, &AJesusBoss2::LockOff);
	}
}

void AJesusBoss2::BeginPlay()
{
	Super::BeginPlay();
	
	SetMetaData();
	AIController = Cast<ABoss2AIController>(GetController());
	Boss2ActionEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("Boss2ActionType"), true);
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

	if (AttackLockOn)
		RotateToPlayerInterp();

	PlayMoveMontage();

	IsGameStart = Boss2AnimInstance->IsStart;
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
		InitIsExcute();

		if (!Boss2->CurrentActionTemp.CanContinuity)
		{
			auto FoundIndex = arr.Find(Boss2->CurrentActionTemp);
			if (FoundIndex == INDEX_NONE)
			{
				CurrentActionTemp = GetRandomPatternMap[AtkType]();
				SetBTAction(CurrentActionTemp);
				return;
			}
			
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
	for (int i = 0; i < LeftActionArr.Num(); i++)
		LeftActionArr[i].IsExcute = false;
	for (int i = 0; i < RightActionArr.Num(); i++)
		RightActionArr[i].IsExcute = false;
	for (int i = 0; i < BackActionArr.Num(); i++)
		BackActionArr[i].IsExcute = false;
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

	LeftAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

	BossDataStruct.CharacterHp -= DamageAmount;

	//TODO : 보스 체력 UI

	//TODO : 본 회전 시키기

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
		switch (PlayerDirection)
		{
		case B2_FOWARD:
			CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_MELEE]();
			break;
		case B2_BACK:
			CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_BACKATK]();
			break;
		case B2_LEFT:
			CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_LEFTATK]();
			break;
		case B2_RIGHT:
			CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_RIGHTATK]();
			break;

		default:
			UE_LOG(LogTemp, Warning, TEXT("PlayerDirection is not set!"));
			break;
		}
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

void AJesusBoss2::LeftCollisionEnableNotify()
{
	LeftAtkCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AJesusBoss2::LeftCollisionDisableNotify()
{
	LeftAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Damage = 0;
}

void AJesusBoss2::RightCollisionEnableNotify()
{
	RightAtkCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AJesusBoss2::RightCollisionDisableNotify()
{
	RightAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Damage = 0;
}

void AJesusBoss2::HeadCollisionEnableNotify()
{
	HeadAtkCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AJesusBoss2::HeadCollisionDisableNotify()
{
	HeadAtkCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Damage = 0;
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

void AJesusBoss2::IsNotifyActive(bool value)
{
}

