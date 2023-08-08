// Fill out your copyright notice in the Description page of Project Settings.

#include "KinghtMonster.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RandomStream.h"
#include "KnightAttackTriggerComp.h"
#include "..\..\Manager\CombatManager.h"
#include "GameFramework/CharacterMovementComponent.h"

AKinghtMonster::AKinghtMonster()
{
	IsDetect = false;

	TargetDetectionCollison = CreateDefaultSubobject<USphereComponent>(TEXT("Target Detection Collider"));
	TargetDetectionCollison->SetupAttachment(RootComponent);
	TargetDetectionCollison->SetCollisionProfileName("Detection");
	TargetDetectionCollison->SetSphereRadius(500.0f);

	ParryingCollision1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Parrying Collision"));
	ParryingCollision1->SetupAttachment(GetMesh(), FName("Weapon_Bone"));

	HpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Hp Widget"));
	HpWidget->SetupAttachment(GetMesh(), FName("Bip001-Spine"));

	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOn Widget"));
	LockOnWidget->SetupAttachment(GetMesh(), FName("Bip001-Spine"));

	SwordTrailComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Sword Trail"));
	SwordTrailComp->SetupAttachment(GetMesh(), FName("Weapon_Bone"));
	SwordTrailComp->SetCollisionProfileName("Sword Trail");

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon2 Box"));
	WeaponCollision->SetupAttachment(GetMesh(), FName("Weapon_Bone"));
	WeaponCollision->SetCollisionProfileName("Weapon");

	WeaponOverlapStaticMeshCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Box"));
	WeaponOverlapStaticMeshCollision->SetupAttachment(GetMesh(), FName("Weapon_Bone"));
	WeaponOverlapStaticMeshCollision->SetCollisionProfileName("Weapon");

	AnimTypeToStateType.Add(KnightAnimationType::FORWARDMOVE, KnightStateType::NONE);
	AnimTypeToStateType.Add(KnightAnimationType::LEFTMOVE, KnightStateType::NONE);
	AnimTypeToStateType.Add(KnightAnimationType::RIGHTMOVE, KnightStateType::NONE);
	AnimTypeToStateType.Add(KnightAnimationType::BACKMOVE, KnightStateType::NONE);

	AnimTypeToStateType.Add(KnightAnimationType::IDLE, KnightStateType::NONE);

	AnimTypeToStateType.Add(KnightAnimationType::ATTACK1, KnightStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(KnightAnimationType::POWERATTACK1, KnightStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(KnightAnimationType::DASHATTACK1, KnightStateType::BEFOREATTACK);

	AnimTypeToStateType.Add(KnightAnimationType::HIT, KnightStateType::CANTACT);
	AnimTypeToStateType.Add(KnightAnimationType::DEAD, KnightStateType::CANTACT);
	AnimTypeToStateType.Add(KnightAnimationType::DEADLOOP, KnightStateType::CANTACT);
	AnimTypeToStateType.Add(KnightAnimationType::EXECUTION, KnightStateType::CANTACT);

	RandomRotateMap.Add(0, [&]()
		{
			AddMovementInput(-((GetActorRotation() - FRotator(0, 0, 0)).Vector() * DiagonalSpeed * fDeltaTime));
		});
	RandomRotateMap.Add(1, [&]()
		{
			AddMovementInput(-((GetActorRotation() - FRotator(0, -45, 0)).Vector() * DiagonalSpeed * fDeltaTime));
		});
	RandomRotateMap.Add(2, [&]()
		{
			AddMovementInput(-((GetActorRotation() - FRotator(0, 45, 0)).Vector() * DiagonalSpeed * fDeltaTime));
		});

	MonsterMoveMap.Add(0, [&]()
		{
			KnightController->Movement(PatrolLocation);
		});
	MonsterMoveMap.Add(1, [&]()
		{
			DiagonalSpeed = KnightDataStruct.CharacterOriginSpeed;
			KnightController->Movement(PlayerCharacter->GetActorLocation());
		});
	MonsterMoveMap.Add(2, [&]()
		{
			DiagonalSpeed = KnightDataStruct.LockOnWalkSpeed * sqrt(0.5f) + KnightDataStruct.LockOnWalkSpeed * sqrt(0.5f);
			//TODO : MonsterRandomMove에 랜덤 값 할당	
			RandomRotateMap[MonsterRandomMove]();
		});

	CheckDIstanceMap.Add(false, [&]()
		{

		});
	CheckDIstanceMap.Add(true, [&]()
		{
			CurrentDistance = FVector::Distance(GetActorLocation(), PlayerCharacter->GetActorLocation());
		});

	MonsterTickEventMap.Add(KnightActionType::NONE, [&]()
	{
	});

	MonsterTickEventMap.Add(KnightActionType::DODGE, [&]()
		{

		});

	MonsterTickEventMap.Add(KnightActionType::ATTACK, [&]()
		{
			RotateMap[true]();
		});

	MonsterTickEventMap.Add(KnightActionType::POWERATTACK, [&]()
		{
			RotateMap[true]();
		});

	MonsterTickEventMap.Add(KnightActionType::MOVE, [&]()
		{
			RotateMap[PlayerCharacter != nullptr]();
			MonsterMoveMap[MonsterMoveEventIndex]();
		});

	MonsterTickEventMap.Add(KnightActionType::HIT, [&]()
		{

		});

	MonsterTickEventMap.Add(KnightActionType::DEAD, [&]()
		{
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
			SetActorTickEnabled(false);

			if (PlayerCharacter != nullptr)
			{
				if (PlayerCharacter->IsLockOn)
				{
					PlayerCharacter->GetCompsInScreen(PlayerCharacter->TargetCompArray);
					PlayerCharacter->GetFirstTarget();
					if (PlayerCharacter->TargetComp == nullptr)
					{
						PlayerCharacter->LockOn();
					}
					else
					{
						Cast<ABaseCharacter>(PlayerCharacter->TargetComp->GetOwner())->ActivateLockOnImage(true);
					}
				}
			}
		});

	MontageEndEventMap.Add(KnightAnimationType::DEAD, [&]()
		{
			ChangeMontageAnimation(KnightAnimationType::DEADLOOP);
			IsStun = true;
			CanExecution = true;
		});

	MontageEndEventMap.Add(KnightAnimationType::DEADLOOP, [&]()
		{
			ChangeMontageAnimation(KnightAnimationType::DEADLOOP);
		});

	MontageEndEventMap.Add(KnightAnimationType::EXECUTION, [&]()
		{
			ChangeActionType(KnightActionType::DEAD);
		});

	MontageEndEventMap.Add(KnightAnimationType::IDLE, [&]()
		{
			if (PlayerCharacter)
			{
				StartAttackTrigger(AttackAnimationType);
			}
			else
			{
				ChangeMontageAnimation(KnightAnimationType::IDLE);
			}
		});

	MontageEndEventMap.Add(KnightAnimationType::ATTACK1, [&]()
		{
			if (TracePlayer)
			{
				MonsterMoveEventIndex = 1;
				ChangeActionType(KnightActionType::MOVE);
				ChangeMontageAnimation(KnightAnimationType::FORWARDMOVE);
			}
			else
			{
				ChangeActionType(KnightActionType::NONE);
				ChangeMontageAnimation(KnightAnimationType::IDLE);
			}
		});

	MontageEndEventMap.Add(KnightAnimationType::POWERATTACK1, MontageEndEventMap[KnightAnimationType::ATTACK1]);
	MontageEndEventMap.Add(KnightAnimationType::DASHATTACK1, MontageEndEventMap[KnightAnimationType::ATTACK1]);

	MontageEndEventMap.Add(KnightAnimationType::HIT, [&]()
		{
			if (TracePlayer)
			{
				MonsterMoveEventIndex = 1;
				ChangeActionType(KnightActionType::MOVE);
				ChangeMontageAnimation(KnightAnimationType::FORWARDMOVE);
			}
			else
			{
				ChangeActionType(KnightActionType::NONE);
				ChangeMontageAnimation(KnightAnimationType::IDLE);
			}
		});

	NotifyBeginEndEventMap.Add(KnightAnimationType::IDLE, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[KnightAnimationType::IDLE].Add(true, [&]()
		{
			if (AttackAnimationType != KnightAnimationType::NONE)
				StartAttackTrigger(AttackAnimationType);

		});
	NotifyBeginEndEventMap[KnightAnimationType::IDLE].Add(false, [&]()
		{
			if (AttackAnimationType != KnightAnimationType::NONE)
				StartAttackTrigger(AttackAnimationType);
		});

	NotifyBeginEndEventMap.Add(KnightAnimationType::ATTACK1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[KnightAnimationType::ATTACK1].Add(true, [&]()
		{
			SwordTrailComp->Activate();
			ParryingCollision1->Activate();
			ActivateSMOverlap();
			ActivateRightWeapon();
			AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[24].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
		});
	NotifyBeginEndEventMap[KnightAnimationType::ATTACK1].Add(false, [&]()
		{
			DeactivateRightWeapon();
			ParryingCollision1->Deactivate();
			SwordTrailComp->Deactivate();
			DeactivateSMOverlap();
		});

	NotifyBeginEndEventMap.Add(KnightAnimationType::POWERATTACK1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[KnightAnimationType::POWERATTACK1].Add(true, NotifyBeginEndEventMap[KnightAnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[KnightAnimationType::POWERATTACK1].Add(false, NotifyBeginEndEventMap[KnightAnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(KnightAnimationType::DASHATTACK1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[KnightAnimationType::DASHATTACK1].Add(true, NotifyBeginEndEventMap[KnightAnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[KnightAnimationType::DASHATTACK1].Add(false, NotifyBeginEndEventMap[KnightAnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(KnightAnimationType::EXECUTION, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[KnightAnimationType::EXECUTION].Add(true, [&]()
		{
		});
	NotifyBeginEndEventMap[KnightAnimationType::EXECUTION].Add(false, [&]()
		{
		});

	RotateMap.Add(false, [&]()
		{

		});
	RotateMap.Add(true, [&]()
		{
			if (PlayerCharacter != nullptr)
			{
				TargetRotation = (PlayerCharacter->GetActorLocation() - GetActorLocation()).Rotation();
				YawRotation = TargetRotation;
			}
		});

	TargetDetectEventMap.Add(KnightAttackType::MELEE, [&]()
		{
			ChangeActionType(KnightActionType::MOVE);
			ChangeMontageAnimation(KnightAnimationType::FORWARDMOVE);
		});
	TargetDetectEventMap.Add(KnightAttackType::RANGE, [&]()
		{
			ChangeActionType(KnightActionType::MOVE);
			ChangeMontageAnimation(KnightAnimationType::FORWARDMOVE);
		});

	SetActionByRandomMap.Add(KnightAnimationType::ATTACK1, [&](float percent)
		{
			if (percent >= 0.5)
			{
				ChangeActionType(KnightActionType::ATTACK);
				ChangeMontageAnimation(KnightAnimationType::ATTACK1);
			}
			else if (percent < 0.5f)
			{
				//UGameplayStatics::SetGlobalTimeDilation(monster, 0.1f);
				ChangeActionType(KnightActionType::ATTACK);
				ChangeMontageAnimation(KnightAnimationType::POWERATTACK1);
			}
		});

	SetActionByRandomMap.Add(KnightAnimationType::DASHATTACK1, [&](float percent)
		{
			if (percent >= 0)
			{
				ChangeActionType(KnightActionType::ATTACK);
				ChangeMontageAnimation(KnightAnimationType::DASHATTACK1);
			}
		});

	//SetActionByRandomMap.Add(KnightAnimationType::DASHATTACK1, [&](float percent)
	//	{
	//		if (percent <= 0.5f)
	//		{
	//			ChangeActionType(KnightActionType::ATTACK);
	//			ChangeMontageAnimation(KnightAnimationType::DASHATTACK1);
	//		}
	//		else
	//		{
	//			ChangeActionType(KnightActionType::MOVE);
	//			ChangeMontageAnimation(KnightAnimationType::FORWARDMOVE);
	//		}
	//	});
}

void AKinghtMonster::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = KnightDataStruct.CharacterOriginSpeed;
	YawRotation = GetActorRotation();

	MonsterHPWidget = Cast<UMonsterHPUI>(HpWidget->GetWidget());
	MonsterLockOnWidget = Cast<UMonsterWidget>(LockOnWidget->GetWidget());

	MonsterLockOnWidget->SetVisibility(ESlateVisibility::Collapsed);

	KnightController = Cast<AKnightController>(GetController());

	UCombatManager::GetInstance().MonsterInfoArray.Add(this);

	//test
	UCombatManager::GetInstance().HitMonsterInfoArray.Add(this);

	PlayerCharacter = nullptr;

	GetCharacterMovement()->bOrientRotationToMovement = false;

	AnimInstance = Cast<UKnightAnimInstance>(GetMesh()->GetAnimInstance());

	AnimationType = KnightAnimationType::IDLE;
	ChangeActionType(KnightActionType::NONE);
	ChangeMontageAnimation(KnightAnimationType::IDLE);

	SwordMeshComp = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

	DeactivateSMOverlap();
	SwordTrailComp->Deactivate();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();

	WeaponOpacity = 0.171653f;
	MeshOpacity = 0.171653f;

	DeactivateHpBar();

	TargetDetectionCollison->OnComponentBeginOverlap.AddDynamic(this, &AKinghtMonster::OnTargetDetectionBeginOverlap);
	TargetDetectionCollison->OnComponentEndOverlap.AddDynamic(this, &AKinghtMonster::OnTargetDetectionEndOverlap);

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AKinghtMonster::OnWeaponOverlapBegin);
	WeaponOverlapStaticMeshCollision->OnComponentBeginOverlap.AddDynamic(this, &AKinghtMonster::OnSMOverlapBegin);
	WeaponOverlapStaticMeshCollision->OnComponentEndOverlap.AddDynamic(this, &AKinghtMonster::OnSMOverlapEnd);

	ParryingCollision1->OnComponentBeginOverlap.AddDynamic(this, &AKinghtMonster::OnParryingOverlap);
}

void AKinghtMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	fDeltaTime = DeltaTime;
	
	CheckDIstanceMap[IsDetect]();
	MonsterTickEventMap[ActionType]();

	Rotate();
}

void AKinghtMonster::ActivateLockOnImage(bool value)
{
	value ? MonsterLockOnWidget->SetVisibility(ESlateVisibility::HitTestInvisible) : MonsterLockOnWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AKinghtMonster::RespawnCharacter()
{
	Super::RespawnCharacter();

	WeaponOpacity = 0.171653f;
	MeshOpacity = 0.171653f;

	ActivateHitCollision();
	KnightDataStruct.CharacterHp = KnightDataStruct.CharacterMaxHp;
	MonsterHPWidget->SetHP(1.0f);
}

void AKinghtMonster::ResumeMontage()
{
	if (MontageMap.Contains(AnimationType))
		AnimInstance->ResumeMontage(MontageMap[AnimationType]);
}

void AKinghtMonster::HitStop()
{
	Super::HitStop();
	if (MontageMap.Contains(AnimationType))
		AnimInstance->PauseAnimation(MontageMap[AnimationType]);
}

void AKinghtMonster::ChangeMontageAnimation(KnightAnimationType type)
{
	if (AnimInstance == nullptr)
	{
		AnimInstance = Cast<UKnightAnimInstance>(GetMesh()->GetAnimInstance());
	}
	else
	{
		AnimInstance->StopMontage(MontageMap[AnimationType]);
		AnimationType = type;
		StateType = AnimTypeToStateType[type];
		AnimInstance->PlayMontage(MontageMap[type]);
	}
}

void AKinghtMonster::ChangeActionType(KnightActionType type)
{
	ActionType = type;
}

void AKinghtMonster::DeactivateHpBar()
{
	MonsterHPWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AKinghtMonster::OnTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActionType == KnightActionType::DEAD)
		return;
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
	}
	TracePlayer = true;
	MonsterMoveEventIndex = 1;
	TargetDetectEventMap[KnightAttackType::MELEE]();
}

void AKinghtMonster::OnTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AttackAnimationType = KnightAnimationType::NONE;
}

void AKinghtMonster::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	if (OtherActor->TakeDamage(SkillInfoMap[AttackAnimationType].Damage, CharacterDamageEvent, nullptr, this))
	{
		HitStop();
		CameraShake(PlayerCameraShake);

		VibrateGamePad(0.4f, 0.4f);

		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[8].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[9].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[31].ObjClass, OtherActor->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
	}
}

void AKinghtMonster::OnSMOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SwordVFXSpawn();
}

void AKinghtMonster::OnSMOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorldTimerManager().ClearTimer(SMOverlapTimerHandler);
}

void AKinghtMonster::OnParryingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Imotal = true;

	KnightDataStruct.CharacterHp -= KnightDataStruct.CharacterHp;
	MonsterHPWidget->DecreaseHPGradual(this, KnightDataStruct.CharacterHp / KnightDataStruct.CharacterMaxHp);

	//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);

	DeactivateSMOverlap();
	DeactivateRightWeapon();
	ParryingCollision1->Deactivate();

	ChangeMontageAnimation(KnightAnimationType::DEAD);

	VibrateGamePad(1.0f, 0.4);
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[6].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[7].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[7].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[3].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
}

void AKinghtMonster::StartAttackTrigger(KnightAnimationType AttackAnimType)
{
	TracePlayer = false;
	if (StateType == KnightStateType::CANTACT || GetMesh()->GetCollisionProfileName() == "Ragdoll")
		return;
	AttackAnimationType = AttackAnimType;
	if (ActionType != KnightActionType::ATTACK)
	{
		KnightController->StopMovement();
		AnimInstance->StopMontage(MontageMap[AnimationType]);
		float RandomValue = FMath::RandRange(0, 100) * 0.01f;
		if (SetActionByRandomMap.Contains(AttackAnimType))
		{
			MonsterMoveEventIndex = 1;
			SetActionByRandomMap[AttackAnimType](RandomValue);
		}
	}
}

void AKinghtMonster::EndAttackTrigger(KnightAnimationType AttackAnimType)
{
	if (AnimationType == KnightAnimationType::DEAD || AnimationType == KnightAnimationType::DEADLOOP)
		return;
	TracePlayer = true;
}

void AKinghtMonster::Rotate()
{
	if (AnimationType == KnightAnimationType::DEAD || AnimationType == KnightAnimationType::DEADLOOP
		|| AnimationType == KnightAnimationType::EXECUTION)
		return;
	SetActorRotation(FMath::Lerp(GetActorRotation(), YawRotation, KnightDataStruct.RotateSpeed * fDeltaTime));
}

float AKinghtMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (Imotal)
	{
		return 0;
	}

	//MonsterHpWidget->Hp->SetVisibility(ESlateVisibility::Visible);
	//MonsterHpWidget->HpBG->SetVisibility(ESlateVisibility::Visible);
	GetWorldTimerManager().SetTimer(HpTimer, this, &AKinghtMonster::DeactivateHpBar, 3.0f);

	DeactivateHitCollision();

	MonsterHPWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	KnightDataStruct.CharacterHp -= DamageAmount;
	MonsterHPWidget->DecreaseHPGradual(this, KnightDataStruct.CharacterHp / KnightDataStruct.CharacterMaxHp);


	if (KnightDataStruct.CharacterHp <= 0)
	{
		Imotal = true;
		KnightController->StopMovement();
		DeactivateSMOverlap();
		ParryingCollision1->Deactivate();
		DeactivateRightWeapon();
		//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
		ChangeMontageAnimation(KnightAnimationType::DEAD);
		return DamageAmount;
	}

	if (DamageAmount >= 30)
	{
		KnightController->StopMovement();
		AnimInstance->StopMontage(MontageMap[AnimationType]);
		if (MontageEndEventMap.Contains(AnimationType))
			MontageEndEventMap[AnimationType]();

		ChangeActionType(KnightActionType::NONE);
		ChangeMontageAnimation(KnightAnimationType::HIT);
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
	ChangeMontageAnimation(KnightAnimationType::EXECUTION);
}
