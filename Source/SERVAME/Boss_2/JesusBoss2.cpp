// Fill out your copyright notice in the Description page of Project Settings.


#include "JesusBoss2.h"
#include "..\Player\PlayerCharacter.h"
#include <random>
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AJesusBoss2::AJesusBoss2()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Boss2HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Boss2 Hit Collision"));
	Boss2HitCollision->SetupAttachment(GetMesh(), FName("LockOn_Bone"));
	Boss2HitCollision->SetCollisionProfileName("AIHit");

	MontageStartMap.Add(Boss2AnimationType::NONE, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
		}));
	MontageEndMap.Add(Boss2AnimationType::NONE, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
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

	//===========================================================================================================
	
	BossAttackMap.Add(Boss2ActionType::B2_FALLTHECROSS, TFunction<void(AJesusBoss2*)>([](AJesusBoss2* Boss2)
		{
			Boss2->DoAttack(Boss2->CurrentActionTemp.Distance, Boss2->MaxAtkRange, 0.f, true, Boss2AnimationType::CROSSFALL, Boss2);
		}));

	//===========================================================================================================

	ActionEndMap.Add(Boss2AttackType::B2_MELEE, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{		
			Boss2ActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);
		
			//TODO : 방향에 맞춰서 GetRandomPatterMap 호출

			if (Dist >= RangeAtk)
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_RANGE]();
			else
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_MELEE]();

			SetBTAction(CurrentActionTemp);
			IsAttackMontageEnd = true;

		}));
	ActionEndMap.Add(Boss2AttackType::B2_RANGE, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{
			Boss2ActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);

			//TODO : 방향에 맞춰서 GetRandomPatterMap 호출

			if (Dist >= RangeAtk)
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_RANGE]();
			else
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_MELEE]();

			SetBTAction(CurrentActionTemp);
			IsAttackMontageEnd = true;
		}));
	ActionEndMap.Add(Boss2AttackType::B2_FOLLOWUP, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{
			Boss2ActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);

			//TODO : 방향에 맞춰서 GetRandomPatterMap 호출

			if (Dist >= RangeAtk)
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_RANGE]();
			else
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_MELEE]();

			SetBTAction(CurrentActionTemp);
			IsAttackMontageEnd = true;
		}));

	ActionEndMap.Add(Boss2AttackType::B2_LEFTATK, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{
			Boss2ActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);

			//TODO : 방향에 맞춰서 GetRandomPatterMap 호출

			if (Dist >= RangeAtk)
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_RANGE]();
			else
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_MELEE]();

			SetBTAction(CurrentActionTemp);
			IsAttackMontageEnd = true;
		}));

	ActionEndMap.Add(Boss2AttackType::B2_RIGHTATK, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{
			Boss2ActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);

			//TODO : 방향에 맞춰서 GetRandomPatterMap 호출

			if (Dist >= RangeAtk)
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_RANGE]();
			else
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_MELEE]();

			SetBTAction(CurrentActionTemp);
			IsAttackMontageEnd = true;
		}));

	ActionEndMap.Add(Boss2AttackType::B2_BACKATK, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{
			Boss2ActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);

			//TODO : 방향에 맞춰서 GetRandomPatterMap 호출

			if (Dist >= RangeAtk)
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_RANGE]();
			else
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_MELEE]();

			SetBTAction(CurrentActionTemp);
			IsAttackMontageEnd = true;
		}));

	//===========================================================================================================
	
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


	//===========================================================================================================

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

	//===========================================================================================================

	ChangePercentageMap.Add(B2_MELEE, TFunction<void(Boss2ActionTemp*)>([=](Boss2ActionTemp* Temp)
		{
		}));


	//===========================================================================================================

	InitPercentageMap.Add(B2_MELEE, TFunction<void()>([=]()
		{
		}));

	//===========================================================================================================

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
	}
}

void AJesusBoss2::BeginPlay()
{
	Super::BeginPlay();
	SetMetaData();
	AIController = Cast<ABoss2AIController>(GetController());

	Boss2ActionEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("Boss2ActionType"), true);

	//테스트 용
	CanMove = true;
	Boss2AnimInstance->IsStart = true;
}

void AJesusBoss2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*=====================
*		Function
=====================*/
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

void AJesusBoss2::DoAttack(float MinRange, float MaxRange, float Dist, bool LockOn, Boss2AnimationType Type, AJesusBoss2* Boss2)
{
	if (Dist >= MinRange && Dist <= MaxRange && !Boss2->CurrentActionTemp.IsExcute)
	{
		//모든 패턴의 IsExcute를 false로 초기화 하는 함수
		InitIsExcute();

		if (!Boss2->CurrentActionTemp.CanContinuity)
		{
			auto FoundIndex = Boss2->MeleeActionArr.Find(Boss2->CurrentActionTemp);
			if (FoundIndex == INDEX_NONE)
			{
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_MELEE]();
				SetBTAction(CurrentActionTemp);
				return;
			}
			Boss2->MeleeActionArr[FoundIndex].IsExcute = true;
		}

		Boss2->IsExplosionPattern = false;
		Boss2->CanMove = false;
		Boss2->GetCharacterMovement()->MaxWalkSpeed = Boss2->CurrentActionTemp.Speed;
		Boss2->PlayAttackAnim(Type);
		Boss2->IsLockOn = LockOn;
		Boss2->IsMontagePlay = false;
	}
	else
	{
		CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_MELEE]();
		SetBTAction(CurrentActionTemp);
	}
}

void AJesusBoss2::DoRangeAttack(float MinRange, float MaxRange, float Dist, bool LockOn, Boss2AnimationType Type, AJesusBoss2* Boss2)
{
	if (Dist >= MinRange && Dist <= MaxRange && !Boss2->CurrentActionTemp.IsExcute)
	{
		InitIsExcute();

		if (!Boss2->CurrentActionTemp.CanContinuity)
		{
			auto FoundIndex = Boss2->RangeActionArr.Find(Boss2->CurrentActionTemp);
			if (FoundIndex == INDEX_NONE)
			{
				CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_RANGE]();
				SetBTAction(CurrentActionTemp);
				return;
			}
			Boss2->RangeActionArr[FoundIndex].IsExcute = true;
		}

		Boss2->CanMove = false;
		Boss2->GetCharacterMovement()->MaxWalkSpeed = Boss2->CurrentActionTemp.Speed;
		Boss2->PlayAttackAnim(Type);
		Boss2->IsLockOn = LockOn;
		Boss2->IsMontagePlay = false;
	}
	else
	{
		CurrentActionTemp = GetRandomPatternMap[Boss2AttackType::B2_RANGE]();
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

/*=====================
		Notify
=====================*/
void AJesusBoss2::OnCrossFall()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (NavSystem == nullptr)
		return;


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
				auto CastObj = Cast<AActor>(PoolObj);
				CastObj->SetActorScale3D(FVector(5.f, 5.f, 5.f));
				CurrentCrossCount++;
			}

			if (CurrentCrossCount >= CrossCount)
			{
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
