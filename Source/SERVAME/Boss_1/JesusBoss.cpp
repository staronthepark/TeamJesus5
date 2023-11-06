// Fill out your copyright notice in the Description page of Project Settings.

#include "JesusBoss.h"
#include "..\Player\PlayerCharacter.h"
#include "Engine/EngineTypes.h"
#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "..\Manager\CombatManager.h"
#include <Misc/OutputDeviceNull.h>
#include <random>
#include "NavigationPath.h"
#include "..\SERVAME.h"

AJesusBoss::AJesusBoss()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	BossWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossWeaponMesh"));
	BossWeaponMesh->SetupAttachment(GetMesh(), FName("Weapon_Bone"));

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("BossSpline"));

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Boss Weapon Box"));
	WeaponCollision->SetupAttachment(BossWeaponMesh);

	BossHitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Boss Hit Collision"));
	BossHitCollision->SetupAttachment(GetMesh(), FName("LockOn_Bone"));
	BossHitCollision->SetCollisionProfileName("AIHit");

	DarkExplosionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("DarkExplosionSphere"));
	DarkExplosionCollider->SetupAttachment(GetMesh());

	SwordTrailComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Sword Trail"));
	SwordTrailComp->SetupAttachment(GetMesh(), FName("Weapon_bone"));

	ParringTrailComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Parring Trail"));
	ParringTrailComp->SetupAttachment(GetMesh(), FName("Weapon_bone"));

	WeaponOverlapStaticMeshCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Boss Overlap Box"));
	WeaponOverlapStaticMeshCollision->SetupAttachment(BossWeaponMesh);

	ParryingCollision1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Parrying Overlap Box"));
	ParryingCollision1->SetupAttachment(BossWeaponMesh);

	GroundExplosionPos = CreateDefaultSubobject<USceneComponent>(TEXT("GroundExplosionPos"));
	GroundExplosionPos->SetupAttachment(BossWeaponMesh);

	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOn Widget"));
	LockOnWidget->SetupAttachment(GetMesh(), FName("Bip001-Spine"));

	DamageSphereTriggerComp = CreateDefaultSubobject<UUDamageSphereTriggerComp>(TEXT("DamageSphere"));
	DamageSphereTriggerComp->SetupAttachment(GetMesh());

	AreaAtkPos = CreateDefaultSubobject<USceneComponent>(TEXT("AreaAtkPos"));
	AreaAtkPos->SetupAttachment(GetMesh());

	SpawnPosArr.SetNum(8);
	for (int i = 0; i < SpawnPosArr.Num(); i++)
	{
		SpawnPosArr[i] = CreateDefaultSubobject<USceneComponent>(*FString::Printf(TEXT("SpawnPos_%d"), i));
		SpawnPosArr[i]->SetupAttachment(GetMesh());
	}

	//static ConstructorHelpers::FClassFinder<UBossUI> BossUIAsset(TEXT("/Game/02_Resource/04_UI/01_WBP/02_BossUI/WBP_BossUI"));

	//if (BossUIAsset.Succeeded())
	//	BossUIClass = BossUIAsset.Class;

	//====================================몽타주 시작/종료 되었을 때 호출되는 코드===========================

	MontageStartMap.Add(BossAnimationType::NONE, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
		}));
	MontageEndMap.Add(BossAnimationType::NONE, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
		}));
	MontageStartMap.Add(BossAnimationType::LEVELSTART, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
		}));
	MontageEndMap.Add(BossAnimationType::LEVELSTART, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsLockOn = true;
		}));

	MontageStartMap.Add(BossAnimationType::IDLE, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->SwordTrailComp->Deactivate();
			Boss->ParringTrailComp->Deactivate();
		}));
	MontageEndMap.Add(BossAnimationType::IDLE, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = true;
			Boss->IsAttacking = false;
			Boss->IdleMontageEndInit(BossAnimationType::IDLE);
		}));

	MontageStartMap.Add(BossAnimationType::CRY, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->SwordTrailComp->Deactivate();
			Boss->ParringTrailComp->Deactivate();
		}));
	MontageEndMap.Add(BossAnimationType::CRY, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IdleMontageEndInit(BossAnimationType::CRY);
		}));

	MontageStartMap.Add(BossAnimationType::HIT, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->SwordTrailComp->Deactivate();
			Boss->ParringTrailComp->Deactivate();
			Boss->GetWorldTimerManager().SetTimer(Boss->HitTimerHandle, Boss, &AJesusBoss::InitHitCount, 8.f);
		}));
	MontageEndMap.Add(BossAnimationType::HIT, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = true;
			Boss->IsAttacking = false;
		}));

	MontageStartMap.Add(BossAnimationType::POWERHIT, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->SwordTrailComp->Deactivate();
			Boss->ParringTrailComp->Deactivate();
			Boss->GetWorldTimerManager().SetTimer(Boss->HitTimerHandle, Boss, &AJesusBoss::InitHitCount, 8.f);
		}));
	MontageEndMap.Add(BossAnimationType::POWERHIT, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = true;
			Boss->IsAttacking = false;
		}));

	MontageStartMap.Add(BossAnimationType::RUN, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->MinWalkTime = Boss->GetRandomNum(2, 4);
			Boss->IsMoveStart = true;
			Boss->SwordTrailComp->Deactivate();
			Boss->ParringTrailComp->Deactivate();
		}));
	MontageEndMap.Add(BossAnimationType::RUN, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsMoveStart = false;
			//Boss->MoveMontageEndInit(BossAnimationType::RUN);
		}));

	MontageStartMap.Add(BossAnimationType::RUN_L, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->MinWalkTime = Boss->GetRandomNum(2, 4);
			Boss->IsMoveStart = true;

			Boss->SwordTrailComp->Deactivate();
			Boss->ParringTrailComp->Deactivate();
		}));
	MontageEndMap.Add(BossAnimationType::RUN_L, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsMoveStart = false;
			//Boss->MoveMontageEndInit(BossAnimationType::RUN_L);
		}));

	MontageStartMap.Add(BossAnimationType::RUN_R, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->MinWalkTime = Boss->GetRandomNum(2, 4);
			Boss->IsMoveStart = true;

			Boss->SwordTrailComp->Deactivate();
			Boss->ParringTrailComp->Deactivate();
		}));
	MontageEndMap.Add(BossAnimationType::RUN_R, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsMoveStart = false;
			//Boss->MoveMontageEndInit(BossAnimationType::RUN_R);
		}));

	MontageStartMap.Add(BossAnimationType::ATTACK, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = false;
			Boss->TestLogStr("ATTACK_START");
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::ATTACK, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->TestLogStr("ATTACK_END");
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			//Boss->DealTimePercent += Boss->GetRandomNum(10, 30);
		}));

	MontageStartMap.Add(BossAnimationType::RANGEATTACK, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::RANGEATTACK, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			//Boss->DealTimePercent += Boss->GetRandomNum(10, 30);
			//Boss->CheckDealTime();
		}));

	MontageStartMap.Add(BossAnimationType::SPRINT, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::SPRINT, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			//Boss->DealTimePercent += Boss->GetRandomNum(10, 30);
			//Boss->CheckDealTime();
		}));

	MontageStartMap.Add(BossAnimationType::UPPERSLASH, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::UPPERSLASH, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			//Boss->DealTimePercent += Boss->GetRandomNum(10, 30);
			//Boss->CheckDealTime();
		}));

	MontageStartMap.Add(BossAnimationType::CHARGEATTACK, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::CHARGEATTACK, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			//Boss->DealTimePercent += Boss->GetRandomNum(10, 30);
			//Boss->CheckDealTime();
		}));

	MontageStartMap.Add(BossAnimationType::BackSwing, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::BackSwing, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			//Boss->DealTimePercent += Boss->GetRandomNum(10, 30);
		}));

	MontageStartMap.Add(BossAnimationType::JUMPATTACK, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
			Boss->DeactivateHitCollision();
		}));
	MontageEndMap.Add(BossAnimationType::JUMPATTACK, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			Boss->Damage = 0;
			Boss->ActivateHitCollision();
			//Boss->DealTimePercent += Boss->GetRandomNum(10, 30);
			//Boss->CheckDealTime();
		}));

	MontageStartMap.Add(BossAnimationType::DARKEXPLOSION, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->DamageSphereTriggerComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::DARKEXPLOSION, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			UE_LOG(LogTemp, Warning, TEXT("asdf"));
			Boss->IsExplosion = false;
			Boss->ExplosionRange = 1;
			Boss->DarkExplosionCollider->SetRelativeScale3D(FVector(Boss->ExplosionRange, Boss->ExplosionRange, Boss->ExplosionRange));
			Boss->DarkExplosionCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->DamageSphereTriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			Boss->CanMove = true;
			Boss->IsLockOn = true;
			//Boss->DealTimePercent += Boss->GetRandomNum(10, 30);

			//Boss->CheckDealTime();
		}));

	MontageStartMap.Add(BossAnimationType::GROUNDEXPLOSION, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::GROUNDEXPLOSION, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			//Boss->DealTimePercent += Boss->GetRandomNum(10, 30);
			Boss->Damage = 0;

			//Boss->CheckDealTime();
		}));

	MontageStartMap.Add(BossAnimationType::DOWNATTACK, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::DOWNATTACK, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			//Boss->GetWorldTimerManager().SetTimer(Boss->TimerHandle, Boss, &AJesusBoss::GroundExplosionCheck, 1.f, true, 1.5f);
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			//Boss->DealTimePercent += Boss->GetRandomNum(10, 30);
			//Boss->CheckDealTime();
		}));

	MontageStartMap.Add(BossAnimationType::FASTSLASH, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::FASTSLASH, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			//Boss->DealTimePercent += Boss->GetRandomNum(10, 30);
			//Boss->CheckDealTime();
		}));

	MontageStartMap.Add(BossAnimationType::THRIDSLASH, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::THRIDSLASH, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			//Boss->DealTimePercent += Boss->GetRandomNum(10, 30);
			//Boss->CheckDealTime();
		}));

	MontageStartMap.Add(BossAnimationType::THIRDJUMP, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
			Boss->DeactivateHitCollision();
		}));
	MontageEndMap.Add(BossAnimationType::THIRDJUMP, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			//Boss->DealTimePercent += Boss->GetRandomNum(10, 30);
			Boss->Damage = 0;
			Boss->ActivateHitCollision();
			//Boss->CheckDealTime();
		}));

	MontageStartMap.Add(BossAnimationType::GROGGY, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->DeactivateSMOverlap();
			Boss->DoStep = false;
			Boss->DamageSphereTriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->IsLockOn = false;
			Boss->CanMove = false;
			Boss->IsAttacking = false;
			Boss->SwordTrailComp->Deactivate();
			Boss->ParringTrailComp->Deactivate();
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::GROGGY, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->IsLockOn = true;
			Boss->IsParriged = false;
			Boss->IsStun = false;
			Boss->CanMove = true;
		}));

	MontageStartMap.Add(BossAnimationType::SHIELD_GROGGY, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->AIController->StopMovement();
			
			Boss->DamageSphereTriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->DoStep = false;
			Boss->ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->DeactivateSMOverlap();
			Boss->IsLockOn = false;
			Boss->CanMove = false;
			Boss->IsAttacking = false;
			Boss->SwordTrailComp->Deactivate();
			Boss->ParringTrailComp->Deactivate();
		}));
	MontageEndMap.Add(BossAnimationType::SHIELD_GROGGY, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->IsLockOn = true;
			Boss->IsParriged = false;
			Boss->IsStun = false;
			Boss->IsMontagePlay = false;
			Boss->CanMove = true;
			Boss->CanExecution = false;
		}));

	MontageStartMap.Add(BossAnimationType::STUN, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->AIController->StopMovement();

			Boss->DoStep = false;
			Boss->DamageSphereTriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->DeactivateSMOverlap();
			Boss->IsLockOn = false;
			Boss->CanMove = false;
			Boss->IsAttacking = false;
			Boss->SwordTrailComp->Deactivate();
			Boss->ParringTrailComp->Deactivate();
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::STUN, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->IsLockOn = true;
			Boss->IsParriged = false;
			Boss->IsStun = false;
			Boss->IsMontagePlay = false;
			Boss->CanMove = true;
		}));

	MontageStartMap.Add(BossAnimationType::GROGGYIDLE, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->AIController->StopMovement();

			Boss->ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->DeactivateSMOverlap();
			Boss->IsLockOn = false;
			Boss->CanMove = false;
			Boss->IsAttacking = false;
			Boss->SwordTrailComp->Deactivate();
			Boss->ParringTrailComp->Deactivate();
			Boss->IsActionEnd = false;
			Boss->IsStun = true;
		}));
	MontageEndMap.Add(BossAnimationType::GROGGYIDLE, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->ChangeMontageAnimation(BossAnimationType::GROGGYIDLE);
		}));

	MontageStartMap.Add(BossAnimationType::EXECUTION, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->AIController->StopMovement();

			Boss->ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->DeactivateSMOverlap();
			Boss->IsStun = false;
			Boss->IsLockOn = false;
			Boss->CanMove = false;
			Boss->IsAttacking = false;
			Boss->SwordTrailComp->Deactivate();
			Boss->ParringTrailComp->Deactivate();
			Boss->IsActionEnd = false;
			Boss->IsExecution = true;
			Boss->IsFirstExecution = false;
		}));
	MontageEndMap.Add(BossAnimationType::EXECUTION, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->IsLockOn = true;
			Boss->IsParriged = false;
			Boss->IsStun = false;
			Boss->CanMove = true;
			Boss->IsMontagePlay = false;
			Boss->IsExecution = true;
			Boss->IsFirstExecution = false;
			Boss->CanExecution = false;
		}));

	MontageStartMap.Add(BossAnimationType::DIE, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->AIController->StopMovement();

			Boss->CurrentAnimType = BossAnimationType::DIE;
			Boss->ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Boss->DeactivateSMOverlap();
			Boss->IsStun = false;
			Boss->IsLockOn = false;
			Boss->CanMove = false;
			Boss->SwordTrailComp->Deactivate();
			Boss->ParringTrailComp->Deactivate();
		}));
	MontageEndMap.Add(BossAnimationType::DIE, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->ChangeMontageAnimation(BossAnimationType::DIE);
		}));

	MontageStartMap.Add(BossAnimationType::BACKSTEP, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->fDeltaTime = 0;
			Boss->IsMoveStart = false;
			Boss->AccumulateDamage = 0;
			Boss->ChangeSuperAction = true;
			Boss->IsLockOn = true;
			Boss->fDeltaTime = 0;
		}));
	MontageEndMap.Add(BossAnimationType::BACKSTEP, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->fDeltaTime = 0;
			Boss->IsMontagePlay = false;
			Boss->DoStep = false;
			Boss->ChangeSuperAction = false;
			Boss->IsAttacking = false;
			Boss->BossSuperAction = SUPER_ATTACK;
			Boss->AIController->GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossBaseAction")), Boss->BossSuperAction);
			Boss->AIController->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsStep")), Boss->DoStep);
		}));

	MontageStartMap.Add(BossAnimationType::RIGHTSTEP, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->fDeltaTime = 0;
			Boss->IsMoveStart = false;
			Boss->AccumulateDamage = 0;
			Boss->ChangeSuperAction = true;
			Boss->IsLockOn = true;

			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::RIGHTSTEP, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->fDeltaTime = 0;
			Boss->IsMontagePlay = false;
			Boss->ChangeSuperAction = false;
			Boss->DoStep = false;
			Boss->IsAttacking = false;
			Boss->BossSuperAction = SUPER_ATTACK;
			Boss->AIController->GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossBaseAction")), Boss->BossSuperAction);
			Boss->AIController->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsStep")), Boss->DoStep);
		
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			Boss->Damage = 0;
		}));

	MontageStartMap.Add(BossAnimationType::LEFTSTEP, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->fDeltaTime = 0;
			Boss->IsMoveStart = false;
			Boss->AccumulateDamage = 0;
			Boss->ChangeSuperAction = true;
			Boss->IsLockOn = true;

			Boss->IsAttackMontageEnd = false;
			Boss->CanMove = false;
			Boss->IsActionEnd = false;
		}));
	MontageEndMap.Add(BossAnimationType::LEFTSTEP, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->fDeltaTime = 0;
			Boss->IsMontagePlay = false;
			Boss->ChangeSuperAction = false;
			Boss->DoStep = false;
			Boss->IsAttacking = false;
			Boss->BossSuperAction = SUPER_ATTACK;
			Boss->AIController->GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossBaseAction")), Boss->BossSuperAction);
			Boss->AIController->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsStep")), Boss->DoStep);
	
			Boss->CanMove = true;
			Boss->IsLockOn = true;
			Boss->Damage = 0;
		}));
	//===========================================공격 실행===================================

	BossAttackMap.Add(BossActionType::M_DOUBLESLASH, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->DoAttack(Boss->CurrentActionTemp.Distance, Boss->MaxAtkRange, 0.f, false, BossAnimationType::ATTACK, Boss);
		}));
	BossAttackMap.Add(BossActionType::M_UPPERSLASH, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->DoAttack(Boss->CurrentActionTemp.Distance, Boss->MaxAtkRange, 0.f, false, BossAnimationType::UPPERSLASH, Boss);
		}));
	BossAttackMap.Add(BossActionType::M_DARKEXPLOSION, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			auto Dist = Boss->AIController->GetBlackboardComponent()->GetValueAsFloat("CalcDist");
			Boss->DoExplosionAttack(Boss->CurrentActionTemp.Distance, Boss->MaxAtkRange, Dist, false, BossAnimationType::DARKEXPLOSION, Boss);
		}));
	BossAttackMap.Add(BossActionType::M_GROUNDEXPLOSION, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->DoExplosionAttack(Boss->CurrentActionTemp.Distance, Boss->MaxAtkRange, 0.f, false, BossAnimationType::GROUNDEXPLOSION, Boss);
		}));

	BossAttackMap.Add(BossActionType::R_THROWSTONE, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			auto Dist = Boss->AIController->GetBlackboardComponent()->GetValueAsFloat("CalcDist");

			Boss->DoRangeAttack(Boss->CurrentActionTemp.Distance, Boss->MaxAtkRange, Dist, true, BossAnimationType::RANGEATTACK, Boss);
		}));
	BossAttackMap.Add(BossActionType::R_JUMPATTACK, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			auto Dist = Boss->AIController->GetBlackboardComponent()->GetValueAsFloat("CalcDist");
			Boss->AttackRadius = 300.f;
			Boss->DoRangeAttack(Boss->CurrentActionTemp.Distance, Boss->MaxAtkRange, Dist, false, BossAnimationType::JUMPATTACK, Boss);
		}));
	BossAttackMap.Add(BossActionType::R_SPRINT, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			auto Dist = Boss->AIController->GetBlackboardComponent()->GetValueAsFloat("CalcDist");
			Boss->DoRangeAttack(Boss->CurrentActionTemp.Distance, Boss->MaxAtkRange, Dist, true, BossAnimationType::SPRINT, Boss);
		}));

	BossAttackMap.Add(BossActionType::F_NEARJUMP, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			auto Dist = Boss->AIController->GetBlackboardComponent()->GetValueAsFloat("CalcDist");
			Boss->DoAttack(Boss->CurrentActionTemp.Distance, Boss->MaxAtkRange, Dist, true, BossAnimationType::CHARGEATTACK, Boss);
		}));
	BossAttackMap.Add(BossActionType::M_DOWNATTACK, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->DoAttack(Boss->CurrentActionTemp.Distance, Boss->MaxAtkRange, 0.f, false, BossAnimationType::DOWNATTACK, Boss);
		}));
	BossAttackMap.Add(BossActionType::F_FASTSLASH, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->DoAttack(Boss->CurrentActionTemp.Distance, Boss->MaxAtkRange, 0.f, false, BossAnimationType::FASTSLASH, Boss);
		}));
	BossAttackMap.Add(BossActionType::F_SLASH, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			Boss->DoAttack(Boss->CurrentActionTemp.Distance, Boss->MaxAtkRange, 0.f, false, BossAnimationType::THRIDSLASH, Boss);
		}));
	BossAttackMap.Add(BossActionType::F_FARJUMP, TFunction<void(AJesusBoss*)>([](AJesusBoss* Boss)
		{
			auto Dist = Boss->AIController->GetBlackboardComponent()->GetValueAsFloat("CalcDist");
			Boss->AttackRadius = 300.f;
			Boss->DoAttack(Boss->CurrentActionTemp.Distance, Boss->MaxAtkRange, 0.f, false, BossAnimationType::THIRDJUMP, Boss);
		}));

	//====================================공격타입에 맞는 공격이 끝났을 때 실행되는 코드====================================

	ActionEndMap.Add(BossAttackType::MELEE, TFunction<void(float, float, UAnimMontage*)>([=](float Dist, float Time, UAnimMontage* Montage)
		{
			BossActionTemp ActionTemp{};

			auto Type = GetTypeFromMetaData(Montage);

			if (Type == BossAnimationType::BACKSTEP || Type == BossAnimationType::LEFTSTEP || Type == BossAnimationType::RIGHTSTEP)
				return;

			if (Type != BossAnimationType::UPPERSLASH)
			{
				if (CurrentActionTemp.IsAddPercentage)
					InitPercentageMap[CurrentActionTemp.AttackType]();
				else
					ChangePercentageMap[CurrentActionTemp.AttackType](&CurrentActionTemp);
			}

			if (PlayerDirection == BACK)
			{
				//CurrentActionTemp
				Attack(BossAnimationType::BackSwing);
				return;
			}

			if (Type == BossAnimationType::ATTACK)
			{
				if (Dist >= 600.f)
					CurrentActionTemp = FollowUpActionArr[0];
				else
					CurrentActionTemp = GetRandomPatternMap[BossAttackType::FOLLOWUP]();

				SetBTAction(CurrentActionTemp);
				IsActionEnd = true;
				return;
			}

			if (IsHalfHp && !IsShowHalfHp)
			{
				IsShowHalfHp = true;
				CurrentActionTemp = MeleeActionArr.Last();
				SetBTAction(CurrentActionTemp);
				IsActionEnd = true;
				return;
			}

			if (Dist >= RangeAtk)
				CurrentActionTemp = GetRandomPatternMap[BossAttackType::RANGE]();
			else
				CurrentActionTemp = GetRandomPatternMap[BossAttackType::MELEE]();

			SetBTAction(CurrentActionTemp);
			IsActionEnd = true;
			IsAttackMontageEnd = true;
		}));

	ActionEndMap.Add(BossAttackType::RANGE, TFunction<void(float, float, UAnimMontage*)>
		([=](float Dist, float Time, UAnimMontage* Montage)
			{
				BossActionTemp ActionTemp{};

				auto Type = GetTypeFromMetaData(Montage);
				
				if (Type == BossAnimationType::BACKSTEP || Type == BossAnimationType::LEFTSTEP || Type == BossAnimationType::RIGHTSTEP)
					return;

				if (CurrentActionTemp.IsAddPercentage)
					InitPercentageMap[CurrentActionTemp.AttackType]();
				else
					ChangePercentageMap[CurrentActionTemp.AttackType](&CurrentActionTemp);

				if (PlayerDirection == BACK)
				{
					Attack(BossAnimationType::BackSwing);
					return;
				}

				if (Type == BossAnimationType::SPRINT)
				{
					Attack(BossAnimationType::UPPERSLASH);
					return;
				}

				if (IsHalfHp && !IsShowHalfHp)
				{
					IsShowHalfHp = true;
					CurrentActionTemp = MeleeActionArr.Last();
					SetBTAction(CurrentActionTemp);
					IsActionEnd = true;
					return;
				}

				if (Dist >= RangeAtk)
					CurrentActionTemp = GetRandomPatternMap[BossAttackType::RANGE]();
				else
					CurrentActionTemp = GetRandomPatternMap[BossAttackType::MELEE]();

				SetBTAction(CurrentActionTemp);
				IsActionEnd = true;
				IsAttackMontageEnd = true;
			}));

	ActionEndMap.Add(BossAttackType::FOLLOWUP, TFunction<void(float, float, UAnimMontage*)>
		([=](float Dist, float Time, UAnimMontage* Montage)
			{
				BossActionTemp ActionTemp{};

				auto Type = GetTypeFromMetaData(Montage);

				if (Type == BossAnimationType::BACKSTEP || Type == BossAnimationType::LEFTSTEP || Type == BossAnimationType::RIGHTSTEP)
					return;

				if (Type != BossAnimationType::THIRDJUMP)
				{
					if (CurrentActionTemp.IsAddPercentage)
						InitPercentageMap[CurrentActionTemp.AttackType]();
					else
						ChangePercentageMap[CurrentActionTemp.AttackType](&CurrentActionTemp);
				}

				if (PlayerDirection == BACK)
				{
					//CurrentActionTemp
					Attack(BossAnimationType::BackSwing);
					return;
				}

				if (IsHalfHp && !IsShowHalfHp)
				{
					IsShowHalfHp = true;
					CurrentActionTemp = MeleeActionArr.Last();
					SetBTAction(CurrentActionTemp);
					IsActionEnd = true;
					return;
				}

				if (Dist >= RangeAtk)
					CurrentActionTemp = GetRandomPatternMap[BossAttackType::RANGE]();
				else
					CurrentActionTemp = GetRandomPatternMap[BossAttackType::MELEE]();

				SetBTAction(CurrentActionTemp);
				IsActionEnd = true;
				IsAttackMontageEnd = true;
			}));

	ActionEndMap.Add(BossAttackType::STEP, TFunction<void(float, float, UAnimMontage*)>
		([=](float Dist, float Time, UAnimMontage* Montage)
			{
				BossActionTemp ActionTemp{};

				auto Type = GetTypeFromMetaData(Montage);

				if (Type == BossAnimationType::BACKSTEP || Type == BossAnimationType::LEFTSTEP || Type == BossAnimationType::RIGHTSTEP)
					return;
			}));


	//======================================확률에 의한 랜덤 패턴 가져오기========================================

	GetRandomPatternMap.Add(BossAttackType::MELEE, TFunction<BossActionTemp()>([=]()
		{
			if (MeleeActionArr.Num() <= 0)
				return BossActionTemp();

			auto RandNum = MakeRandom(MeleePercentageVec);

			auto PatternName = BossActionEnum->GetNameStringByValue(MeleeActionArr[RandNum].ActionType);
			//UE_LOG(LogTemp, Warning, TEXT("======================================"));
			//UE_LOG(LogTemp, Warning, TEXT("MeleeRandom : %s"), *PatternName);
			//UE_LOG(LogTemp, Warning, TEXT("PatternProbability : %d"), MeleeActionArr[RandNum].Percentage);
			//UE_LOG(LogTemp, Warning, TEXT("IsAdded : %s"), MeleeActionArr[RandNum].IsAddPercentage ? TEXT("TRUE") : TEXT("FALSE"));
			//UE_LOG(LogTemp, Warning, TEXT("======================================"));

			return  MeleeActionArr[RandNum];
		}));

	GetRandomPatternMap.Add(BossAttackType::RANGE, TFunction<BossActionTemp()>([=]()
		{
			if (RangeActionArr.Num() <= 0)
				return BossActionTemp();

			auto RandNum = MakeRandom(RangePercentageVec);

			auto PatternName = BossActionEnum->GetNameStringByValue(RangeActionArr[RandNum].ActionType);
			//UE_LOG(LogTemp, Warning, TEXT("======================================"));
			//UE_LOG(LogTemp, Warning, TEXT("RangeRandom : %s"), *PatternName);
			//UE_LOG(LogTemp, Warning, TEXT("PatternProbability : %d"), RangeActionArr[RandNum].Percentage);
			//UE_LOG(LogTemp, Warning, TEXT("IsAdded : %s"), RangeActionArr[RandNum].IsAddPercentage ? TEXT("TRUE") : TEXT("FALSE"));
			//UE_LOG(LogTemp, Warning, TEXT("======================================"));

			return RangeActionArr[RandNum];
		}));

	GetRandomPatternMap.Add(BossAttackType::FOLLOWUP, TFunction<BossActionTemp()>([=]()
		{
			if (FollowUpActionArr.Num() <= 0)
				return BossActionTemp();

			auto RandNum = MakeRandom(FollowUpPercentageVec);

			auto PatternName = BossActionEnum->GetNameStringByValue(FollowUpActionArr[RandNum].ActionType);
			//UE_LOG(LogTemp, Warning, TEXT("======================================"));
			//UE_LOG(LogTemp, Warning, TEXT("FollowUpRandom : %s"), *PatternName);
			//UE_LOG(LogTemp, Warning, TEXT("PatternProbability : %d"), FollowUpActionArr[RandNum].Percentage);
			//UE_LOG(LogTemp, Warning, TEXT("IsAdded : %s"), FollowUpActionArr[RandNum].IsAddPercentage ? TEXT("TRUE") : TEXT("FALSE"));
			//UE_LOG(LogTemp, Warning, TEXT("======================================"));

			return  FollowUpActionArr[RandNum];
		}));
	//=============================================================================================

	CheckBoolNExcuteFunctionMap.Add(true, TFunction<void()>([=]()
		{
		}));

	//===================================배열안에 패턴 추가==============================================
	AddArrMap.Add(MELEE, TFunction<void(BossActionTemp*)>([=](BossActionTemp* Temp)
		{
			MeleeActionArr.Add(*Temp);
			MeleePercentageVec.push_back(Temp->Percentage);
		}));

	AddArrMap.Add(RANGE, TFunction<void(BossActionTemp*)>([=](BossActionTemp* Temp)
		{
			RangeActionArr.Add(*Temp);
			RangePercentageVec.push_back(Temp->Percentage);
		}));

	AddArrMap.Add(FOLLOWUP, TFunction<void(BossActionTemp*)>([=](BossActionTemp* Temp)
		{
			FollowUpActionArr.Add(*Temp);
			FollowUpPercentageVec.push_back(Temp->Percentage);
		}));

	AddArrMap.Add(STEP, TFunction<void(BossActionTemp*)>([=](BossActionTemp* Temp)
		{
			StepArr.Add(*Temp);
		}));

	//====================================확률 변경============================================

	ChangePercentageMap.Add(MELEE, TFunction<void(BossActionTemp*)>([=](BossActionTemp* Temp)
		{
			MeleePercentageVec.clear();

			for (int i = 0; i < MeleeActionArr.Num(); i++)
			{
				if (MeleeActionArr[i].ActionType == BossActionType::M_UPPERSLASH)
				{
					MeleePercentageVec.push_back(MeleeActionArr[i].Percentage);
					continue;
				}
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
	ChangePercentageMap.Add(RANGE, TFunction<void(BossActionTemp*)>([=](BossActionTemp* Temp)
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
	ChangePercentageMap.Add(FOLLOWUP, TFunction<void(BossActionTemp*)>([=](BossActionTemp* Temp)
		{
			FollowUpPercentageVec.clear();

			for (int i = 0; i < FollowUpActionArr.Num(); i++)
			{
				if (FollowUpActionArr[i].ActionType == BossActionType::F_FARJUMP)
				{
					FollowUpPercentageVec.push_back(FollowUpActionArr[i].Percentage);
					continue;
				}
				if (FollowUpActionArr[i].ActionType == Temp->ActionType)
				{
					FollowUpActionArr[i].Percentage -= DecreasePercentageVal;
					FollowUpPercentageVec.push_back(FollowUpActionArr[i].Percentage);
					continue;
				}

				FollowUpActionArr[i].IsAddPercentage = true;
				FollowUpActionArr[i].Percentage += DecreasePercentageVal / (FollowUpActionArr.Num() - 2);
				FollowUpPercentageVec.push_back(FollowUpActionArr[i].Percentage);
			}
		}));

	ChangePercentageMap.Add(STEP, TFunction<void(BossActionTemp*)>([=](BossActionTemp* Temp)
		{

		}));

	//===============================확률 초기화========================================

	InitPercentageMap.Add(MELEE, TFunction<void()>([=]()
		{
			MeleePercentageVec.clear();

			for (int i = 0; i < MeleeActionArr.Num(); i++)
			{
				auto BossActionName = BossActionEnum->GetNameStringByValue(MeleeActionArr[i].ActionType);
				MeleeActionArr[i].Percentage = GetActionData(*BossActionName)->Percentage;
				MeleeActionArr[i].IsAddPercentage = false;
				MeleePercentageVec.push_back(MeleeActionArr[i].Percentage);
			}
		}));
	InitPercentageMap.Add(RANGE, TFunction<void()>([=]()
		{
			RangePercentageVec.clear();

			for (int i = 0; i < RangeActionArr.Num(); i++)
			{
				auto BossActionName = BossActionEnum->GetNameStringByValue(RangeActionArr[i].ActionType);
				RangeActionArr[i].Percentage = GetActionData(*BossActionName)->Percentage;
				RangeActionArr[i].IsAddPercentage = false;
				RangePercentageVec.push_back(RangeActionArr[i].Percentage);
			}
		}));
	InitPercentageMap.Add(FOLLOWUP, TFunction<void()>([=]()
		{
			FollowUpPercentageVec.clear();

			for (int i = 0; i < FollowUpActionArr.Num(); i++)
			{
				auto BossActionName = BossActionEnum->GetNameStringByValue(FollowUpActionArr[i].ActionType);
				FollowUpActionArr[i].Percentage = GetActionData(*BossActionName)->Percentage;
				FollowUpActionArr[i].IsAddPercentage = false;
				FollowUpPercentageVec.push_back(FollowUpActionArr[i].Percentage);
			}
		}));

	//=======================================플레이어 공격타입에 따른 피격 모션=========================================

	HitMap.Add(ActionType::ATTACK, TFunction<void()>([=]()
		{
			if (IsStun)
				return;

			if (HitCount >= 2)
				return;

			HitCount++;

			AObjectPool& objectpool = AObjectPool::GetInstance();
			for (int32 i = 0; i < BossDataStruct.DropSoulCount; i++)
			{
				float x = FMath::RandRange(-300.0f, 300.0f);
				float y = FMath::RandRange(-300.0f, 300.0f);
				float z = FMath::RandRange(-300.0f, 300.0f);

				FVector location = GetActorLocation() + FVector(x * 0.1f, y * 0.1f, z * 0.1f);
				FRotator rotation = GetActorRotation() + FRotator(x, y, z);

				objectpool.SpawnObject(objectpool.ObjectArray[36].ObjClass, location, rotation);
			}

			ChangeMontageAnimation(BossAnimationType::HIT);
		}));

	HitMap.Add(ActionType::POWERATTACK, TFunction<void()>([=]()
		{
			if (IsStun)
				return;

			if (HitCount >= 2)
				return;

			HitCount++;

			AObjectPool& objectpool = AObjectPool::GetInstance();
			for (int32 i = 0; i < BossDataStruct.DropSoulCount; i++)
			{
				float x = FMath::RandRange(-300.0f, 300.0f);
				float y = FMath::RandRange(-300.0f, 300.0f);
				float z = FMath::RandRange(-300.0f, 300.0f);

				FVector location = GetActorLocation() + FVector(x * 0.1f, y * 0.1f, z * 0.1f);
				FRotator rotation = GetActorRotation() + FRotator(x, y, z);

				objectpool.SpawnObject(objectpool.ObjectArray[36].ObjClass, location, rotation);
			}
			ChangeMontageAnimation(BossAnimationType::POWERHIT);
		}));

	HitMap.Add(ActionType::SKILL, TFunction<void()>([=]()
		{
			if (IsStun)
				return;

			if (HitCount >= 2)
				return;

			HitCount++;

			AObjectPool& objectpool = AObjectPool::GetInstance();
			for (int32 i = 0; i < BossDataStruct.DropSoulCount; i++)
			{
				float x = FMath::RandRange(-300.0f, 300.0f);
				float y = FMath::RandRange(-300.0f, 300.0f);
				float z = FMath::RandRange(-300.0f, 300.0f);

				FVector location = GetActorLocation() + FVector(x * 0.1f, y * 0.1f, z * 0.1f);
				FRotator rotation = GetActorRotation() + FRotator(x, y, z);

				objectpool.SpawnObject(objectpool.ObjectArray[36].ObjClass, location, rotation);
			}
			ChangeMontageAnimation(BossAnimationType::POWERHIT);
		}));

	//============================================================================================

	HitEffectRotatorList.Add(BossAnimationType::ATTACK, FRotator(-60, 90.0f, 0));
	HitEffectRotatorList.Add(BossAnimationType::THRIDSLASH, FRotator(50, 90.0f, 0));
	HitEffectRotatorList.Add(BossAnimationType::FASTSLASH, FRotator(50, 90.0f, 0));
	HitEffectRotatorList.Add(BossAnimationType::CHARGEATTACK, FRotator(0, 90.0f, 0));
	HitEffectRotatorList.Add(BossAnimationType::UPPERSLASH, FRotator(0, 90.0f, 0));
	HitEffectRotatorList.Add(BossAnimationType::DOWNATTACK, FRotator(-90, 90.0f, 0));
}

AJesusBoss::~AJesusBoss()
{
	MeleePercentageVec.clear();
	RangePercentageVec.clear();

	MeleePercentageVec.shrink_to_fit();
	RangePercentageVec.shrink_to_fit();

	FollowUpPercentageVec.clear();
	FollowUpPercentageVec.shrink_to_fit();
}

void AJesusBoss::EndSequence()
{
	GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(PlayerCharacter, 100.0f);
	PlayerCharacter->PlayerHUD->SetVisibility(ESlateVisibility::Visible);

	ASoundManager::GetInstance().PlaySoundWithCymbalSound(BGMType::TITLEVERSE2, true);

	FLatentActionInfo LatentInfo;
	UGameplayStatics::UnloadStreamLevel(this, "A_KimMinYeongMap_Boss1", LatentInfo, false);
	UGameplayStatics::UnloadStreamLevel(this, "2-2Map", LatentInfo, false);
	UCombatManager::GetInstance().MonsterInfoMap["Boss2PhaseMap"][0]->RespawnCharacter();
}

void AJesusBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BossAnimInstance = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());

	if (BossAnimInstance)
	{
		BossAnimInstance->OnStart.AddUObject(this, &AJesusBoss::OnStart);
		BossAnimInstance->OnEnd.AddUObject(this, &AJesusBoss::OnEnd);
		BossAnimInstance->OnLevelStart.AddUObject(this, &AJesusBoss::LevelStart);
		BossAnimInstance->OnRangeAttack.AddUObject(this, &AJesusBoss::TempRangeNotify);
		BossAnimInstance->OnAttackCheck.AddUObject(this, &AJesusBoss::AttackCheck);
		BossAnimInstance->OnEnable.AddUObject(this, &AJesusBoss::CollisionEnableNotify);
		BossAnimInstance->OnDisable.AddUObject(this, &AJesusBoss::CollisionDisableNotify);
		BossAnimInstance->OnImmediatelyLockOn.AddUObject(this, &AJesusBoss::RotateToPlayer);
		BossAnimInstance->OnLockOn.AddUObject(this, &AJesusBoss::SetLockOn);
		BossAnimInstance->OnLockOff.AddUObject(this, &AJesusBoss::SetLockOff);
		BossAnimInstance->OnChangeDamage.AddUObject(this, &AJesusBoss::ChangeDamage);
		BossAnimInstance->OnOverlapEnable.AddUObject(this, &AJesusBoss::ActivateSMOverlap);
		BossAnimInstance->OnOverlapDisable.AddUObject(this, &AJesusBoss::DeactivateSMOverlap);
		BossAnimInstance->OnTrailActivate.AddUObject(this, &AJesusBoss::TrailOn);
		BossAnimInstance->OnTrailDeActivate.AddUObject(this, &AJesusBoss::TrailOff);
		BossAnimInstance->OnExplosion.AddUObject(this, &AJesusBoss::ExplosionNotify);
		BossAnimInstance->OnJumpMove.AddUObject(this, &AJesusBoss::SlerpJump);
		BossAnimInstance->OnJumpMoveEnd.AddUObject(this, &AJesusBoss::SlerpJumpEnd);
		BossAnimInstance->OnSetParticleLoc.AddUObject(this, &AJesusBoss::SetParticleLastLoc);
		BossAnimInstance->OnReturnStun.AddUObject(this, &AJesusBoss::ReturnStun);
		BossAnimInstance->OnExecution.AddUObject(this, &AJesusBoss::ExecutionNotify);

		BossAnimInstance->OnMontageEnded.AddDynamic(this, &AJesusBoss::GetEndedMontage);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("AnimInstance IS NULL!"));

	OnHit.AddUObject(this, &AJesusBoss::ChangeHitStatus);
	//WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss::SpearHit);
}

void AJesusBoss::BeginPlay()
{
	Super::BeginPlay();

	UCombatManager::GetInstance().AddMonsterInfo(this);

	BossActionEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("BossActionType"), true);

	AIController = Cast<ABossAIController>(GetController());

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss::AttackHit);
	
	DarkExplosionCollider->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss::DarkExplosionHit);

	WeaponOverlapStaticMeshCollision->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss::OnSMOverlapBegin);
	WeaponOverlapStaticMeshCollision->OnComponentEndOverlap.AddDynamic(this, &AJesusBoss::OnSMOverlapEnd);

	ParryingCollision1->OnComponentBeginOverlap.AddDynamic(this, &AJesusBoss::OnParryingOverlap);

	GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DarkExplosionCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageSphereTriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MonsterLockOnWidget = Cast<UMonsterWidget>(LockOnWidget->GetWidget());
	/*==========
	*	UI
	===========*/
	//if (IsValid(BossUIClass))
	//{
	//	BossUI = Cast<UBossUI>(CreateWidget(GetWorld(), BossUIClass));

	//	if (IsValid(BossUI))
	//	{
	//		BossUI->AddToViewport();
	//		BossUI->SetVisibility(ESlateVisibility::Hidden);
	//	}
	//}
	////GameInstance->PlayerHUDWidget->PB_BossHP->SetPercent((float)BossDataStruct.CharacterHp / (float)BossDataStruct.CharacterMaxHp);
	//BossUI->SetHP(1);//

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	if (BossRoomDoorOpenSequence != nullptr)
	{
		BossRoomDoorOpenSequncePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), BossRoomDoorOpenSequence
			, PlaybackSettings, LevelSequenceActor);

		BossRoomDoorOpenSequncePlayer->OnFinished.AddDynamic(this, &AJesusBoss::EndSequence);
	}

	CineCameraActor = Cast<ACineCameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACineCameraActor::StaticClass()));

	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("PlayEndingCredit"), ar, NULL, true);
	   
	DeactivateSMOverlap();

	ChangeMontageAnimation(BossAnimationType::LEVELSTART);

	GetCharacterMovement()->MaxWalkSpeed = BossDataStruct.CharacterOriginSpeed;
	BossDataStruct.CurrentGrrogyGauge = BossDataStruct.MaxGrrogyGauge;

	SetMetaData();
	SwordTrailComp->Deactivate();
	ParringTrailComp->Deactivate();
	CurrentAnimType = BossAnimationType::NONE;

	TArray<UActorComponent*>ActorCompArray;
	ActorCompArray = GetComponentsByTag(USphereComponent::StaticClass(), FName("LockOnTarget"));
	LockOnComp = Cast<USphereComponent>(ActorCompArray[0]);

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MonsterLockOnWidget->LockOnImage->SetVisibility(ESlateVisibility::Hidden);
}

void AJesusBoss::Tick(float DeltaTime)
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

	if (IsMoveStart && BossSuperAction == SUPER_ATTACK)
	{
		fDeltaTime += DeltaTime;
		if (fDeltaTime >= MinWalkTime)
			DoRandomStep();
	}

	if (GetVelocity().Size() > 0)
		CurrentStatus = MOVE;
	else if (GetVelocity().Size() < 0 && CurrentStatus != HIT)
		CurrentStatus = NOTIHIG;
	     
	if (AttackLockOn)
		RotateToPlayerInterp();

	if (IsExplosion)
	{
		if (ExplosionRange >= 100)
			return;
		ExplosionRange += 2.f;
		DarkExplosionCollider->SetRelativeScale3D(FVector(ExplosionRange, ExplosionRange, ExplosionRange));
	}

	if (JumpMoveStart)
		JumpMove();

	CheckBossDie();

	PlayMoveMontage();
	//디버그 UI

	//if (GroundExplosionCnt >= 3)
	//{
	//	GetWorldTimerManager().ClearTimer(TimerHandle);
	//	GroundExplosionCnt = 0;
	//	AttackRadius = 400;
	//}

	if (BossDataStruct.CharacterHp <= BossDataStruct.CharacterMaxHp / 2 && IsHalfHp == false)
	{
		IsHalfHp = true;
	}

	//기존 피격 모션(본 회전시키는 코드)
	if (IsStartBoneRot)
		StartBoneRot();
	else
	{
		BossAnimInstance->BoneRotateVal = FMath::RInterpTo(BossAnimInstance->BoneRotateVal, 
			BossAnimInstance->CurrentBoneRotateVal, ReturnDeltaSeconds, ReturnSpeed);
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}

	IsGameStart = BossAnimInstance->IsStart;
}

void AJesusBoss::PlayMoveMontage()
{
	if (BossAnimInstance->Speed > 50 && !IsAttacking && !DoStep && !DealTime && IsPlayerAlive
		&& AIController->GetBlackboardComponent()->GetValueAsBool("CanMove"))
	{
		if (CurrentAnimType == BossAnimationType::DIE)
			return;

		if (CurrentAnimType != BossAnimationType::RUN_L && CurrentAnimType != BossAnimationType::RUN_R)
			ChangeAnimType(BossAnimationType::RUN);

		if (CurrentAnimType == BossAnimationType::IDLE)
			ChangeAnimType(BossAnimationType::RUN);

		if (!IsMontagePlay)
		{
			if (!IsParriged)
			{
				ChangeMontageAnimation(CurrentAnimType);
			}
			IsMontagePlay = true;
		}
	}
	else
	{
		if (!BossAnimInstance->IsAnyMontagePlaying())
		{
			ChangeMontageAnimation(BossAnimationType::IDLE);
			IsMontagePlay = false;
		}
		else
		{
			if (BossAnimInstance->GetCurrentActiveMontage() != nullptr)
			{
				FString  test = BossAnimInstance->GetCurrentActiveMontage()->GetName();
				if (test == "AM_BossRun" || test == "AM_BossRun_L" || test == "AM_BossRun_R")
				{
					IsMontagePlay = false;
				}
			}
		}
	}
}

#include "../Player/JesusPlayerController.h"

void AJesusBoss::CheckBossDie()
{	
	if (BossDataStruct.CharacterHp <= 0 && IsDead == false)
	{
		ASoundManager::GetInstance().PauseBGM();


		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, "Boss2PhaseMap", true, true, LatentInfo);
		PlayerCharacter->CurrentMapName = "Boss2PhaseMap";

		DamageSphereTriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PlayerCharacter->AxisX = 1;
		PlayerCharacter->AxisY = 1;
		ChangeMontageAnimation(BossAnimationType::DIE);
		IsDead = true;
		AIController->BossUI->PlayBossDiedAnimtion();
		AIController->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsDetected")), false);
		AIController->DetectedActorArr.Empty();
		AIController->OnUnPossess();

		//for (auto iter = BossDataStruct.DamageList.begin(); iter != BossDataStruct.DamageList.end(); iter.operator++())
		//{
		//	iter.Value() = 0;
		//}

		//맵 로딩 됐을 때 BossUI의 PlayFadeOutAnimation 호출
		GetWorldTimerManager().SetTimer(ChangePlayerLocTimerHandle, FTimerDelegate::CreateLambda([=]()
			{
				AJesusPlayerController* controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());

				controller->CurrentSequncePlayer = BossRoomDoorOpenSequncePlayer;
				BossRoomDoorOpenSequncePlayer->Play();
				PlayerCharacter->PlayerHUD->SetVisibility(ESlateVisibility::Collapsed);
				GetWorld()->GetFirstPlayerController()->DisableInput(GetWorld()->GetFirstPlayerController());
				GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(CineCameraActor, 6.0f);
				PlayerCharacter->MoveSpawnLocation(FVector(-6912.570360, -60620.187377, 40.0f));
				//PlayerCharacter->SetActorLocation();
			}), 2.f, false);

		GetWorldTimerManager().SetTimer(FadeInTimerHandle, FTimerDelegate::CreateLambda([=]()
			{


				UE_LOG(LogTemp, Warning, TEXT("FADEOUTANIMATION"));
				AIController->BossUI->PlayFadeOutAnimation();
				//AIController->BossUI->SetVisibility(ESlateVisibility::Hidden);
			}), 2.5f, false);

		GetWorldTimerManager().SetTimer(FadeInTimerHandle, FTimerDelegate::CreateLambda([=]()
			{
				AIController->BossUI->RemoveFromParent();
			}), 2.6f, false);
	}
}

void AJesusBoss::IdleMontageEndInit(BossAnimationType Type)
{
	if (DealTimePercent >= 100)
	{
		DealTime = false;
		DealTimePercent = 0;
		CanMove = true;
		BossSuperAction = SUPER_ATTACK;
		AIController->GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossBaseAction")), BossSuperAction);
	}
	else
	{
		ChangeMontageAnimation(Type);
	}
}

void AJesusBoss::MoveMontageEndInit(BossAnimationType Type)
{
	ChangeMontageAnimation(Type);
}

void AJesusBoss::CheckDealTime()
{
	if (DealTimePercent >= 100 )
	{
		DealTime = true;
		BossSuperAction = SUPER_MOVE;
		ChangeSuperAction = true;
		CanMove = false;
		AIController->GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossBaseAction")), BossSuperAction);
		ChangeMontageAnimation(BossAnimationType::IDLE);
		UE_LOG(LogTemp, Warning, TEXT("DealTime!"));
	}
}

void AJesusBoss::SetBTAction(BossActionTemp Temp)
{
	AIController->GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossActionType")), Temp.ActionType);
}

int AJesusBoss::GetRandomNum(int Min, int Max)
{
	std::srand(time(NULL));
	auto Val = rand() % Max + Min;
	return Val;
}

void AJesusBoss::DoRandomStep()
{
	UE_LOG(LogTemp, Warning, TEXT("fDeltaTime : %f"), fDeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("MinWalkTime : %f"), MinWalkTime);

	IsMoveStart = false;
	fDeltaTime = 0.f;
	srand(time(NULL));

	//todo : 스텝 두개를 패턴화 시켜서 히트 캔슬 막아주기

	switch (rand() % 2)
	{
	case 0:
		DoStep = true;
		CurrentActionTemp = StepArr[1];
		BossAnimInstance->PlayMontage(BossMontageMap[BossAnimationType::RIGHTSTEP]);
		break;

	case 1:
		DoStep = true;
		CurrentActionTemp = StepArr[0];
		BossAnimInstance->PlayMontage(BossMontageMap[BossAnimationType::LEFTSTEP]);
		break;
	}
}

void AJesusBoss::JumpMove()
{
	auto NewLoc = Lerp(GetActorLocation(), LastPlayerLoc, 0.05f);
	SetActorLocation(NewLoc);
}

FVector AJesusBoss::Lerp(const FVector& start, const FVector& end, const float t)
{
	FVector NewLoc = FMath::Lerp(start, end, t);

	return NewLoc;
}

void AJesusBoss::ChangeHitStatus()
{
	CurrentStatus = HIT;
}

void AJesusBoss::HitStop()
{
	Super::HitStop();

	if (BossAnimInstance->GetCurrentActiveMontage() != nullptr)
		BossAnimInstance->PauseAnimation(BossAnimInstance->GetCurrentActiveMontage());
}

void AJesusBoss::ResumeMontage()
{
	if (BossAnimInstance->GetCurrentActiveMontage() != nullptr)
		BossAnimInstance->ResumeMontage(BossAnimInstance->GetCurrentActiveMontage());
}

void AJesusBoss::ChangeMontageAnimation(BossAnimationType Type)
{
	BossAnimInstance->PlayMontage(BossMontageMap[Type]);
	ChangeAnimType(Type);
}

void AJesusBoss::ChangeAnimType(BossAnimationType Type)
{
	CurrentAnimType = Type;
}

void AJesusBoss::RotateToPlayer()
{
	FRotator ToTarget = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerCharacter->GetActorLocation());
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Yaw, 0.f);
	SetActorRotation(LookAtRotation);
}

void AJesusBoss::RotateToPlayerInterp()
{
	FRotator ToTarget = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerCharacter->GetActorLocation());
	FRotator LookAtRotation = FMath::RInterpTo(GetActorRotation(), ToTarget, GetWorld()->DeltaTimeSeconds, 4.f);
	SetActorRotation(LookAtRotation);
}

float AJesusBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser))
		return 0.0f;

	AccumulateDamage += DamageAmount;
	
	BossDataStruct.CharacterHp -= DamageAmount;
	
	AIController->BossUI->DecreaseHPGradual(this, BossDataStruct.CharacterHp / BossDataStruct.CharacterMaxHp);
	AIController->BossUI->SetDamageText(DamageAmount);
	DeactivateHitCollision();

	bool IsAnimTest = AIController->GetBlackboardComponent()->GetValueAsBool("IsAnimTest");
	
	if (IsAnimTest)
		return DamageAmount;

	//기존 피격 모션(본 회전시키는 코드)
	IsStartBoneRot = true;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AJesusBoss::ReSetBoneRot, Time, false);
	
	//if (BossDataStruct.CharacterHp <= 2000 && IsExecution == false && IsHitStun == false)
	//{
	//	if (GetTypeFromMetaData(StartMontage) == BossAnimationType::DARKEXPLOSION ||
	//		GetTypeFromMetaData(StartMontage) == BossAnimationType::GROUNDEXPLOSION)
	//		return DamageAmount;

	//	IsStun = true;
	//	IsLockOn = false;
	//	AttackLockOn = false;
	//	IsParriged = true;
	//	IsFirstExecution = true;
	//	IsHitStun = true;
	//	BossDataStruct.CurrentGrrogyGauge = 0;
	//	BossAnimInstance->PlayGroggyMontage(BossAnimationType::STUN);
	//}

	if (!StartEnd.Get<0>() && CurrentActionTemp.HitCancel)
	{
		if(HitMap.Contains(PlayerCharacter->PlayerAttackType))
			HitMap[PlayerCharacter->PlayerAttackType]();
	}

	return DamageAmount;
}

void AJesusBoss::StartBoneRot()
{
	BossAnimInstance->BoneRotateVal = FMath::RInterpTo(BossAnimInstance->BoneRotateVal, BoneRotVal, DeltaSeconds, Speed);
}

void AJesusBoss::ReSetBoneRot()
{
	IsStartBoneRot = false;
}

void AJesusBoss::SetSplineCurve()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	//나중에 비동기로 변경
	auto Result = NavSystem->FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), PlayerCharacter->GetActorLocation());

	FNavPathSharedPtr InPath = Result->GetPath();

	if (InPath.IsValid())
	{
		const TArray<FNavPathPoint>& PathPoints = InPath->GetPathPoints();

		for (const FNavPathPoint& PathPoint : PathPoints)
		{
			Spline->AddSplinePoint(PathPoint.Location, ESplineCoordinateSpace::World, ESplinePointType::Curve);
		}

		Spline->UpdateSpline();
	}
}

void AJesusBoss::InitHitCount()
{
	HitCount = 0;
	GetWorldTimerManager().ClearTimer(HitTimerHandle);
}

void AJesusBoss::PlayMonsterSoundInPool(EMonsterAudioType AudioType)
{
	AObjectPool& objectpool = AObjectPool::GetInstance();

	auto Obj = objectpool.SpawnObject(objectpool.ObjectArray[MONSTERSOUNDOP].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
	auto MonsterSound = Cast<AMonsterSoundObjectInpool>(Obj);

	MonsterSound->PlayMonsterSound(AudioType);
}

void AJesusBoss::RespawnCharacter()
{
	Super::RespawnCharacter();
	SpawnInit();
}

void AJesusBoss::PlayExecutionAnimation()
{
	CanExecution = false;
	ChangeMontageAnimation(BossAnimationType::EXECUTION);
}

void AJesusBoss::ActivateLockOnImage(bool value, UPrimitiveComponent* comp)
{
	value ? MonsterLockOnWidget->SetVisibility(ESlateVisibility::HitTestInvisible) : MonsterLockOnWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AJesusBoss::Stun()
{	
	IsParriged = true;
	AttackLockOn = false;
	BossDataStruct.CurrentGrrogyGauge = 0;
	BossAnimInstance->PlayGroggyMontage(BossAnimationType::SHIELD_GROGGY);
}

bool AJesusBoss::IsAlive()
{
	if(BossDataStruct.CharacterHp > 0)
	return true;
	return false;
}

void AJesusBoss::ActivateSMOverlap()
{
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[12].ObjClass, WeaponOverlapStaticMeshCollision->GetComponentLocation(), FRotator(90, 0, 0));
	GetWorldTimerManager().SetTimer(SMOverlapTimerHandler, this, &AJesusBoss::ActivateSMOverlap, 0.02f);
}

void AJesusBoss::DeactivateSMOverlap()
{
	GetWorldTimerManager().ClearTimer(SMOverlapTimerHandler);
}

void AJesusBoss::SpawnInit()
{
	UE_LOG(LogTemp, Warning, TEXT("Boss1 Respawn"));

	AIController->OnPossess(this);

	//스텟
	BossDataStruct.CharacterHp = BossDataStruct.CharacterMaxHp;
	BossDataStruct.CurrentGrrogyGauge = BossDataStruct.MaxGrrogyGauge;
	AIController->BossUI->SetHP(1);
	BossDataStruct.CharacterOriginSpeed = 120.f;
	AccumulateDamage = 0.f;
	DealTimePercent = 0;
	IsExecution = false;
	IsFirstExecution = false;
	IsHitStun = false;
	IsHalfHp = false;
	IsShowHalfHp = false;
	IsDead = false;
	Push2PhasePattern = false;
	CanExecution = false;
	CanMove = true;
	BossAnimInstance->IsStart = true;

	//패턴확률
	InitPercentageMap[BossAttackType::MELEE]();
	InitPercentageMap[BossAttackType::RANGE]();
	InitPercentageMap[BossAttackType::FOLLOWUP]();

	//2페이즈 패턴 제거
	BossActionTemp DarkTemp, GroundTemp;
	DarkTemp.ActionType = BossActionType::M_DARKEXPLOSION;
	GroundTemp.ActionType = BossActionType::M_GROUNDEXPLOSION;

	auto FoundIndex1 = MeleeActionArr.Find(DarkTemp);
	if (FoundIndex1 != -1)
		MeleeActionArr.RemoveAt(FoundIndex1);
	auto FoundIndex2 = MeleeActionArr.Find(GroundTemp);
	if (FoundIndex2 != -1)
		MeleeActionArr.RemoveAt(FoundIndex2);

	//애니
	ChangeMontageAnimation(BossAnimationType::IDLE);

	//BT
	if (!MeleeActionArr.IsEmpty())
	{
		CurrentActionTemp = MeleeActionArr[0];
		AIController->GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossActionType")), CurrentActionTemp.ActionType);
	}
	AIController->GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossBaseAction")), SUPER_MOVE);
}

void AJesusBoss::GroundExplosionCheck()
{
	GroundExplosionCnt += 1;

	if (GroundExplosionCnt == 1)
		AttackRadius = 400.f;
	else if (GroundExplosionCnt == 2)
		AttackRadius = 700.f;
	else if (GroundExplosionCnt == 3)
		AttackRadius = 1000.f;

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		ParticleLoc,
		ParticleLoc,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	FColor DrawColor;

	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	//DrawDebugSphere(GetWorld(), ParticleLoc, AttackRadius, 16, DrawColor, false, 2.f);

	CameraShake(PlayerCameraShake);
	VibrateGamePad(1.0f, 0.5f);

	if (bResult && HitResult.GetActor())
	{
		//UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName());
		FDamageEvent DamageEvent;
		auto Player = Cast<APlayerCharacter>(HitResult.GetActor());
	
		Player->TakeDamage(BossDataStruct.DamageList[BossAnimationType::DOWNATTACK], DamageEvent, GetController(), this);
	}
}

void AJesusBoss::DoAttack(float MinRange, float MaxRange, float Dist, bool LockOn, BossAnimationType Type, AJesusBoss* Boss)
{
	if (Dist >= MinRange && Dist <= MaxRange && !Boss->CurrentActionTemp.IsExcute)
	{
		//모든 패턴의 IsExcute를 false로 초기화 하는 함수
		InitIsExcute();

		if (!Boss->CurrentActionTemp.CanContinuity)
		{
			auto FoundIndex = Boss->MeleeActionArr.Find(Boss->CurrentActionTemp);
			if (FoundIndex == INDEX_NONE)
			{
				CurrentActionTemp = GetRandomPatternMap[BossAttackType::MELEE]();
				SetBTAction(CurrentActionTemp);
				return;
			}
			Boss->MeleeActionArr[FoundIndex].IsExcute = true;
		}
		
		Boss->IsExplosionPattern = false;
		Boss->CanMove = false;
		Boss->GetCharacterMovement()->MaxWalkSpeed = Boss->CurrentActionTemp.Speed;
		Boss->Attack(Type);
		Boss->IsLockOn = LockOn;
		Boss->IsMontagePlay = false;
	}
	else
	{
		if (CurrentActionTemp.ActionType == F_NEARJUMP)
		{
			//BossAnimInstance->StopMontage(BossAnimInstance->GetCurrentActiveMontage());
			CurrentActionTemp = GetRandomPatternMap[BossAttackType::FOLLOWUP]();
			SetBTAction(CurrentActionTemp);
			return;
		}

		//BossAnimInstance->StopMontage(BossAnimInstance->GetCurrentActiveMontage());
		CurrentActionTemp = GetRandomPatternMap[BossAttackType::MELEE]();
		SetBTAction(CurrentActionTemp);
	}
}

void AJesusBoss::DoRangeAttack(float MinRange, float MaxRange, float Dist, bool LockOn, BossAnimationType Type, AJesusBoss* Boss)
{
	if (Dist >= MinRange && Dist <= MaxRange && !Boss->CurrentActionTemp.IsExcute)
	{
		InitIsExcute();

		if (!Boss->CurrentActionTemp.CanContinuity)
		{
			auto FoundIndex = Boss->RangeActionArr.Find(Boss->CurrentActionTemp);
			if (FoundIndex == INDEX_NONE)
			{
				CurrentActionTemp = GetRandomPatternMap[BossAttackType::RANGE]();
				SetBTAction(CurrentActionTemp);
				return;
			}
			Boss->RangeActionArr[FoundIndex].IsExcute = true;
		}

		Boss->CanMove = false;
		Boss->GetCharacterMovement()->MaxWalkSpeed = Boss->CurrentActionTemp.Speed;
		Boss->Attack(Type);
		Boss->IsLockOn = LockOn;
		Boss->IsMontagePlay = false;
	}
	else
	{
		//BossAnimInstance->StopMontage(BossAnimInstance->GetCurrentActiveMontage());
		CurrentActionTemp = GetRandomPatternMap[BossAttackType::RANGE]();
		SetBTAction(CurrentActionTemp);
	}
}

void AJesusBoss::DoExplosionAttack(float MinRange, float MaxRange, float Dist, bool LockOn, BossAnimationType Type, AJesusBoss* Boss)
{
	if (Dist >= MinRange && Dist <= MaxRange && !Boss->CurrentActionTemp.IsExcute && !Boss->IsExplosionPattern)
	{
		InitIsExcute();

		if (!Boss->CurrentActionTemp.CanContinuity)
		{
			auto FoundIndex = Boss->MeleeActionArr.Find(Boss->CurrentActionTemp);
			if (FoundIndex == INDEX_NONE)
			{
				CurrentActionTemp = GetRandomPatternMap[BossAttackType::MELEE]();
				SetBTAction(CurrentActionTemp);
				return;
			}
			Boss->MeleeActionArr[FoundIndex].IsExcute = true;
		}

		Boss->IsExplosionPattern = true;
		Boss->CanMove = false;
		Boss->GetCharacterMovement()->MaxWalkSpeed = Boss->CurrentActionTemp.Speed;
		Boss->Attack(Type);
		Boss->IsLockOn = LockOn;
		Boss->IsMontagePlay = false;
	}
	else
	{
		CurrentActionTemp = GetRandomPatternMap[BossAttackType::MELEE]();
		SetBTAction(CurrentActionTemp);
	}
}

void AJesusBoss::Attack(BossAnimationType Type)
{
	if (IsAttacking)
		return;

	CurrentStatus = ATTACK;

	IsAttacking = true;

	BossAnimInstance->StopAllMontages(0.25f);
	ChangeMontageAnimation(Type);
}

void AJesusBoss::InitIsExcute()
{
	for (int i = 0; i < MeleeActionArr.Num(); i++)
		MeleeActionArr[i].IsExcute = false;
	for (int i = 0; i < RangeActionArr.Num(); i++)
		RangeActionArr[i].IsExcute = false;
	for (int i = 0; i < FollowUpActionArr.Num(); i++)
		FollowUpActionArr[i].IsExcute = false;
}

FBossAction* AJesusBoss::GetActionData(FName Name)
{
	return	BossActions->FindRow<FBossAction>(Name, TEXT(""));
}

void AJesusBoss::SetMetaData()
{
	BossAnimationType Type;
	UAnimMontage* Montage;

	for (auto iter = BossMontageMap.begin(); iter != BossMontageMap.end(); iter.operator++())
	{
		Type = iter.Key();
		Montage = iter.Value();

		Montage->EmptyMetaData();
		UBossAnimMetaData* MetaData = NewObject<UBossAnimMetaData>();
		MetaData->AnimType = Type;
		Montage->AddMetaData(MetaData);
	}
}

BossAnimationType AJesusBoss::GetTypeFromMetaData(UAnimMontage* Montage)
{
	auto MetaDataArr = Montage->GetMetaData();
	
	if (MetaDataArr.Num() == 0)
		return BossAnimationType::NONE;

	return Cast<UBossAnimMetaData>(MetaDataArr[0])->AnimType;
}

/*======================
*		UFUNCTION
======================*/
void AJesusBoss::GetEndedMontage(UAnimMontage* Montage, bool bInterrupted)
{
	IsStart.Exchange(false);
	StartEnd.Key = false;
	StartEnd.Value = true;
	DoStep = false;
	IsExplosion = false;

	auto Type = GetTypeFromMetaData(Montage);

	if (Type == BossAnimationType::RUN || Type == BossAnimationType::RUN_L || Type == BossAnimationType::RUN_R || Type == BossAnimationType::SPRINT)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OnMontageEnded : %s"), *Montage->GetName());
		
		if (Montage->GetName() == "AM_BossStart")
		{
			IsStart.Exchange(false);
			return;
		}

		IsMoveStart = false;
	}
}

void AJesusBoss::AttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FDamageEvent DamageEvent;
	
	auto Player = Cast<APlayerCharacter>(OtherActor);
	if (Player == nullptr)
		return;
	
	//UE_LOG(LogTemp, Warning, TEXT("PlayerTakeDamage"));
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	auto Type = GetTypeFromMetaData(StartMontage);
	
	AObjectPool& objectpool = AObjectPool::GetInstance();
	if (OtherComp->GetName() == "ShieldCollision")
	{
		if (BossDataStruct.DamageList.Contains(Type))
			Player->SetShieldHP(-BossDataStruct.DamageList[Type], GetActorLocation());
		CameraShake(PlayerCameraShake);
		VibrateGamePad(0.4f, 0.4f);
		objectpool.SpawnObject(objectpool.ObjectArray[6].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		objectpool.SpawnObject(objectpool.ObjectArray[19].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		return;
	}

	if (!Player->Imotal)
	{
		if (BossDataStruct.DamageList.Contains(Type))
			Damage += BossDataStruct.DamageList[Type];
		else
			return;

		if (OtherActor->TakeDamage(Damage, DamageEvent, GetController(), this))
		{

			//UE_LOG(LogTemp, Warning, TEXT("DAMAGE : %d"), Damage);
			objectpool.SpawnObject(objectpool.ObjectArray[6].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
			AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[31].ObjClass, OtherActor->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);

			if (HitEffectRotatorList.Contains(Type))
			{
				if (Type == BossAnimationType::ATTACK && !CheckAttack2.Exchange(true))
				{
					AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[5].ObjClass,
						OtherComp->GetComponentLocation(), HitEffectRotatorList[GetTypeFromMetaData(StartMontage)]);
				}
				else if (Type == BossAnimationType::ATTACK && CheckAttack2.Exchange(false))
				{
					AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[5].ObjClass,
						OtherComp->GetComponentLocation(), FRotator(-180, 90.0f, 0));
				}
				else
					AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[5].ObjClass,
						OtherComp->GetComponentLocation(), HitEffectRotatorList[GetTypeFromMetaData(StartMontage)]);
			}
		}
	}
}

void AJesusBoss::OnSMOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//SwordVFXSpawn();
}

void AJesusBoss::OnSMOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GetWorldTimerManager().ClearTimer(SMOverlapTimerHandler);
}

void AJesusBoss::OnParryingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CurrentActionTemp.CanParring)
		return;

	IsParriged = true;
	AttackLockOn = false;
	CameraShake(PlayerCameraShake);
	BossDataStruct.CurrentGrrogyGauge -= 50.f;
	BossAnimInstance->StopMontage(BossAnimInstance->GetCurrentActiveMontage());
	BossAnimInstance->PlayGroggyMontage(BossDataStruct.CurrentGrrogyGauge <= 0 ?
		BossAnimationType::STUN : BossAnimationType::GROGGY);
	VibrateGamePad(1.0f, 0.4);
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[6].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[7].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[7].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[3].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
}

void AJesusBoss::DarkExplosionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FDamageEvent DamageEvent;
	//auto ExplosionDamage = BossDataStruct.DamageList[GetTypeFromMetaData(GetCurrentMontage())];
	auto ExplosionDamage = BossDataStruct.DamageList[BossAnimationType::DARKEXPLOSION];
	
	auto Player = Cast<APlayerCharacter>(OtherActor);
	if (Player == nullptr)
		return;
	
	Player->TakeDamage(ExplosionDamage, DamageEvent, GetController(), this);
}

void AJesusBoss::GroundExplosionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FDamageEvent DamageEvent;
	//auto ExplosionDamage = BossDataStruct.DamageList[GetTypeFromMetaData(GetCurrentMontage())];
	auto ExplosionDamage = BossDataStruct.DamageList[BossAnimationType::DOWNATTACK];
	
	auto Player = Cast<APlayerCharacter>(OtherActor);
	if (Player == nullptr)
		return;
	
	Player->TakeDamage(ExplosionDamage, DamageEvent, GetController(), this);
}

/*=====================
		DebugLog
=====================*/

void AJesusBoss::ActionEndMapTestLog(FString Str, float f)
{			
	UE_LOG(LogTemp, Warning, TEXT("========================================="));
	UE_LOG(LogTemp, Warning, TEXT("ActionName : %s"), *Str);
	UE_LOG(LogTemp, Warning, TEXT("ActionCooltime : %f"), f);
	UE_LOG(LogTemp, Warning, TEXT("========================================="));
}

/*======================
* 애니메이션 노티파이
======================*/
void AJesusBoss::LevelStart()
{
	CanMove = true;
	BossAnimInstance->IsStart = true;
	IsActionEnd = true;
	//ChangeMontageAnimation(BossAnimationType::IDLE);
	IsMontagePlay = false;
}

void AJesusBoss::TempRangeNotify()
{
	//
	for (int i = 0; i < SpawnPosArr.Num(); i++)
	{
		std::default_random_engine RandomEngine;
		std::uniform_real_distribution<float> RandomScaleY(0.2f, 0.3f);
		std::uniform_real_distribution<float> RandomScaleZ(0.8f, 1.f);

		float ScaleTempY = RandomScaleY(RandomEngine);
		float ScaleTempZ = RandomScaleZ(RandomEngine);

		auto PoolObject = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[4].ObjClass,
			SpawnPosArr[i]->GetComponentLocation(), SpawnPosArr[i]->GetComponentRotation());
		auto CastObject = Cast<AActor>(PoolObject);
		CastObject->SetActorScale3D(FVector(1.f, ScaleTempY, ScaleTempZ));
	}
}

void AJesusBoss::ExplosionNotify()
{
	IsExplosion = true;
	DarkExplosionCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

//범위로 충돌 체크
void AJesusBoss::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	FVector Center = GetActorLocation();
	FColor DrawColor;

	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	//DrawDebugSphere(GetWorld(), Center, AttackRadius, 16, DrawColor, false, 1.f);

	CameraShake(PlayerCameraShake);
	VibrateGamePad(1.0f, 0.5f);
		
	if (bResult && HitResult.GetActor())
	{
		//UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName());
		FDamageEvent DamageEvent;
		auto Player = Cast<APlayerCharacter>(HitResult.GetActor());
	
		Player->TakeDamage(BossDataStruct.DamageList[GetTypeFromMetaData(GetCurrentMontage())], DamageEvent, GetController(), this);
	}
}

void AJesusBoss::CollisionEnableNotify()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ParringTrailComp->Activate();
}

void AJesusBoss::CollisionDisableNotify()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParringTrailComp->Deactivate();
	Damage = 0;
}

void AJesusBoss::TrailOn()
{
	if (CurrentActionTemp.CanParring)
	{ 
		ParringTrailComp->Activate();
	}
	else
		SwordTrailComp->Activate();
}

void AJesusBoss::TrailOff()
{
	SwordTrailComp->Deactivate();
}

void AJesusBoss::SetLockOn()
{
	AttackLockOn = true;
}

void AJesusBoss::SetLockOff()
{
	AttackLockOn = false;
	LastPlayerLoc = PlayerCharacter->GetActorLocation();
}

void AJesusBoss::ChangeDamage()
{
	Damage += 30;
}

void AJesusBoss::SlerpJump() 
{	
	GetCapsuleComponent()->SetCollisionProfileName("IgnorePlayer");
	JumpMoveStart = true;
	DeactivateHitCollision();
}

void AJesusBoss::SlerpJumpEnd() 
{ 
	JumpMoveStart = false;
	GetCapsuleComponent()->SetCollisionProfileName("AIPhysics");
	ActivateHitCollision();
}

void AJesusBoss::OnStart()
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

void AJesusBoss::OnEnd()
{
	StartEnd.Key = false;
	StartEnd.Value = true;

	GetCapsuleComponent()->SetCollisionProfileName("AIPhysics");
	IsAttacking = false;
	AttackLockOn = false;
	SwordTrailComp->Deactivate();
	//ParringTrailComp->Deactivate();
	FRotator NewRotation(0.f, GetActorRotation().Yaw, 0.f);
	SetActorRotation(NewRotation);

	auto Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	float Dist = 0.0f;
	if(Player)
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

void AJesusBoss::SetParticleLastLoc() { ParticleLoc = GroundExplosionPos->GetComponentLocation(); }

void AJesusBoss::ReturnStun()
{	
	if (IsFirstExecution)
		ChangeMontageAnimation(BossAnimationType::GROGGYIDLE);

	//피격 그로기 다시 생기면 아래 코드 지워야 됨.
	CanExecution = false;
}

void AJesusBoss::IsNotifyActive(bool value)
{
	fDeltaTime = 0;
}

void AJesusBoss::ExecutionNotify()
{
	CanExecution = true;
}
