// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMonster.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RandomStream.h"
#include "MonsterAttackTriggerComp.h"
#include "..\Manager\CombatManager.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyMonster::AEnemyMonster()
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

	GrabCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Grab Damage"));
	GrabCollision->SetupAttachment(GetMesh());
	GrabCollision->SetCollisionProfileName("Grab Damage");

	GrabShieldCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Grab Guard"));
	GrabShieldCollision->SetupAttachment(GetMesh());
	GrabShieldCollision->SetCollisionProfileName("Grab Guard");

	AnimTypeToStateType.Add(MonsterAnimationType::FORWARDMOVE, MonsterStateType::NONE);
	AnimTypeToStateType.Add(MonsterAnimationType::LEFTMOVE, MonsterStateType::NONE);
	AnimTypeToStateType.Add(MonsterAnimationType::RIGHTMOVE, MonsterStateType::NONE);
	AnimTypeToStateType.Add(MonsterAnimationType::BACKMOVE, MonsterStateType::NONE);
	AnimTypeToStateType.Add(MonsterAnimationType::STANDBY, MonsterStateType::NONE);

	AnimTypeToStateType.Add(MonsterAnimationType::IDLE, MonsterStateType::NONE);

	AnimTypeToStateType.Add(MonsterAnimationType::ATTACK1, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::POWERATTACK1, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::RANGEATTACK1, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::DASHATTACK1, MonsterStateType::BEFOREATTACK);

	AnimTypeToStateType.Add(MonsterAnimationType::HIT, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::DEAD, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::DEADLOOP, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::EXECUTION, MonsterStateType::CANTACT);


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
	RandomRotateMap.Add(3, [&]()
		{
			
		});

	MonsterMoveMap.Add(0, [&]()
		{
			MonsterController->Movement(PatrolLocation);
		});
	MonsterMoveMap.Add(1, [&]()
		{			
			DiagonalSpeed = MonsterDataStruct.CharacterOriginSpeed;
			MonsterController->Movement(PlayerCharacter->GetActorLocation());
		});
	MonsterMoveMap.Add(2, [&]()
		{
			DiagonalSpeed = MonsterDataStruct.LockOnWalkSpeed * sqrt(0.5f) + MonsterDataStruct.LockOnWalkSpeed * sqrt(0.5f);
			RandomRotateMap[MonsterRandomMove]();
		});


	CheckDIstanceMap.Add(false, [&]()
		{
			
		});
	CheckDIstanceMap.Add(true, [&]()
		{
			CurrentDistance = FVector::Distance(GetActorLocation(), PlayerCharacter->GetActorLocation());
		});

	MonsterActionEventMap.Add(MonsterStateType::NONE, TMap<MonsterActionType, TFunction<void()>>());
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::NONE, [&]()
		{
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::DODGE, [&]()
		{

		});
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::ATTACK, [&]()
		{
			ChangeActionType(MonsterActionType::ATTACK);
			ChangeMontageAnimation(AttackAnimationType);
		});
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::POWERATTACK, [&]()
		{

		});
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::MOVE, [&]()
		{
			ChangeActionType(MonsterActionType::MOVE);
			ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
		});
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::HIT, [&]()
		{

		});
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::DEAD, [&]() {});

	MonsterActionEventMap.Add(MonsterStateType::BEFOREATTACK, TMap<MonsterActionType, TFunction<void()>>());
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::NONE, [&]() {});
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::DODGE, [&]() {});
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::ATTACK, [&]() {});
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::POWERATTACK, [&]() {});
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::MOVE, [&]() {});
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::HIT, [&]() {});
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::DEAD, [&]() {});

	MonsterActionEventMap.Add(MonsterStateType::AFTERATTACK, TMap<MonsterActionType, TFunction<void()>>());
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::NONE, [&]() {});
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::DODGE, [&]() {});
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::ATTACK, [&]() {});
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::POWERATTACK, [&]() {});
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::MOVE, [&]() {});
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::HIT, [&]() {});
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::DEAD, [&]() {});

	MonsterActionEventMap.Add(MonsterStateType::CANTACT, TMap<MonsterActionType, TFunction<void()>>());
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::NONE, [&]() {});
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::DODGE, [&]() {});
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::ATTACK, [&]() {});
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::POWERATTACK, [&]() {});
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::MOVE, [&]() {});
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::HIT, [&]() {});
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::DEAD, [&]() {});

	MonsterTickEventMap.Add(MonsterActionType::NONE, [&]()
		{
			//test
			RotateMap[PlayerCharacter != nullptr]();
		});

	MonsterTickEventMap.Add(MonsterActionType::DODGE, [&]()
		{

		});

	MonsterTickEventMap.Add(MonsterActionType::ATTACK, [&]()
		{
			RotateMap[true]();
		});

	MonsterTickEventMap.Add(MonsterActionType::POWERATTACK, [&]()
		{
			RotateMap[true]();
		});

	MonsterTickEventMap.Add(MonsterActionType::MOVE, [&]()
		{
			RotateMap[PlayerCharacter != nullptr]();
			MonsterMoveMap[MonsterMoveEventIndex]();
		});

	MonsterTickEventMap.Add(MonsterActionType::HIT, [&]()
		{

		});

	MonsterTickEventMap.Add(MonsterActionType::DEAD, [&]()
		{
			SkeletalMeshComp->SetScalarParameterValueOnMaterials("Opacity", MeshOpacity -= fDeltaTime * 0.25f);
			SwordMeshComp->SetScalarParameterValueOnMaterials("Opacity", WeaponOpacity  -= fDeltaTime * 0.25f);
			if (WeaponOpacity < 0.0f)
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
			}
		});	


	MontageEndEventMap.Add(MonsterAnimationType::DEAD, [&]()
		{
			ChangeMontageAnimation(MonsterAnimationType::DEADLOOP);
			IsStun = true;
			CanExecution = true;
		});

	MontageEndEventMap.Add(MonsterAnimationType::DEADLOOP, [&]()
		{
			ChangeMontageAnimation(MonsterAnimationType::DEADLOOP);
		});

	MontageEndEventMap.Add(MonsterAnimationType::EXECUTION, [&]()
		{
			ChangeActionType(MonsterActionType::DEAD);
		});

	MontageEndEventMap.Add(MonsterAnimationType::STANDBY, [&]()
		{
			ChangeMontageAnimation(MonsterAnimationType::STANDBY);
		});

	MontageEndEventMap.Add(MonsterAnimationType::IDLE, [&]()
		{
			if (PlayerCharacter)
			{
				StartAttackTrigger(AttackAnimationType);
			}
			else
			{
				ChangeMontageAnimation(MonsterAnimationType::IDLE);
			}
		});

	MontageEndEventMap.Add(MonsterAnimationType::ATTACK1, [&]()
		{			
			if (TracePlayer)
			{
				MonsterMoveEventIndex = 1;
				ChangeActionType(MonsterActionType::MOVE);
				ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
			}
			else
			{
				ChangeActionType(MonsterActionType::NONE);
				ChangeMontageAnimation(MonsterAnimationType::IDLE);
			}				
		});
	
	MontageEndEventMap.Add(MonsterAnimationType::POWERATTACK1, MontageEndEventMap[MonsterAnimationType::ATTACK1]);
	MontageEndEventMap.Add(MonsterAnimationType::RANGEATTACK1, MontageEndEventMap[MonsterAnimationType::ATTACK1]);
	MontageEndEventMap.Add(MonsterAnimationType::DASHATTACK1, MontageEndEventMap[MonsterAnimationType::ATTACK1]);

	MontageEndEventMap.Add(MonsterAnimationType::HIT, [&]()
		{
			UE_LOG(LogTemp, Warning, TEXT("!@#!@#"));
			if (TracePlayer)
			{
				MonsterMoveEventIndex = 1;
				ChangeActionType(MonsterActionType::MOVE);
				ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
			}
			else
			{
				ChangeActionType(MonsterActionType::NONE);
				ChangeMontageAnimation(MonsterAnimationType::IDLE);
			}
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::IDLE, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::IDLE].Add(true, [&]()
		{
			if (AttackAnimationType != MonsterAnimationType::NONE)
				StartAttackTrigger(AttackAnimationType);

		});
	NotifyBeginEndEventMap[MonsterAnimationType::IDLE].Add(false, [&]()
		{
			if (AttackAnimationType != MonsterAnimationType::NONE)
				StartAttackTrigger(AttackAnimationType);
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::ATTACK1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1].Add(true, [&]()
		{
			SwordTrailComp->Activate();
			ParryingCollision1->Activate();
			ActivateSMOverlap();
			ActivateRightWeapon();
			AObjectPool& objectpool = AObjectPool::GetInstance();
			objectpool.SpawnObject(objectpool.ObjectArray[24].ObjClass, GetActorLocation(), FRotator::ZeroRotator);

		});
	NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1].Add(false, [&]()
		{
			DeactivateRightWeapon();
			ParryingCollision1->Deactivate();
			SwordTrailComp->Deactivate();
			DeactivateSMOverlap();
		});	

	NotifyBeginEndEventMap.Add(MonsterAnimationType::POWERATTACK1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::POWERATTACK1].Add(true,  NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[MonsterAnimationType::POWERATTACK1].Add(false, NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(MonsterAnimationType::DASHATTACK1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::DASHATTACK1].Add(true, NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[MonsterAnimationType::DASHATTACK1].Add(false, NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(MonsterAnimationType::RANGEATTACK1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::RANGEATTACK1].Add(true, [&]()
		{
			NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][true]();
			ShotProjectile();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::RANGEATTACK1].Add(false, [&]()
		{
			NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][false]();
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::EXECUTION, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::EXECUTION].Add(true, [&]()
		{
		});
	NotifyBeginEndEventMap[MonsterAnimationType::EXECUTION].Add(false, [&]()
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

	TargetDetectEventMap.Add(MonsterAttackType::MELEE, [&]()
		{
			MonsterActionEventMap[MonsterStateType::NONE][MonsterActionType::MOVE]();
		});
	TargetDetectEventMap.Add(MonsterAttackType::RANGE, [&]()
		{
			MonsterActionEventMap[MonsterStateType::NONE][MonsterActionType::MOVE]();
		});

	SetActionByRandomMap.Add(MonsterAnimationType::ATTACK1, [&](float percent)
		{
			if (percent >= 0.5)
			{
				PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::dodge, true);
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::ATTACK1);
			}
			else if (percent < 0.5f)
			{
				//UGameplayStatics::SetGlobalTimeDilation(monster, 0.1f);
				PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::parrying, true);
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::POWERATTACK1);
			}
		});
	SetActionByRandomMap.Add(MonsterAnimationType::DASHATTACK1, [&](float percent)
		{
			if (percent <= 0.5f)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::DASHATTACK1);
			}
			else
			{
				ChangeActionType(MonsterActionType::MOVE);
				ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
			}
		});
	SetActionByRandomMap.Add(MonsterAnimationType::RANGEATTACK1, [&](float percent)
		{
			if (percent <= 0.8f)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::RANGEATTACK1);
			}
			else
			{
				ChangeActionType(MonsterActionType::NONE);
				ChangeMontageAnimation(MonsterAnimationType::IDLE);
			}
		});
}

void AEnemyMonster::BeginPlay()
{
	Super::BeginPlay();


	GetCharacterMovement()->MaxWalkSpeed = MonsterDataStruct.CharacterOriginSpeed;
	YawRotation = GetActorRotation();

	MonsterHPWidget = Cast<UMonsterHPUI>(HpWidget->GetWidget());
	MonsterLockOnWidget = Cast<UMonsterWidget>(LockOnWidget->GetWidget());

	MonsterLockOnWidget->SetVisibility(ESlateVisibility::Collapsed);

	MonsterController = Cast<AMonsterController>(GetController());

	UCombatManager::GetInstance().MonsterInfoArray.Add(this);

	//test
	UCombatManager::GetInstance().HitMonsterInfoArray.Add(this);
		
	PlayerCharacter = nullptr;
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
	

	AnimInstance = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());

	AnimationType = MonsterAnimationType::IDLE;
	ChangeActionType(MonsterActionType::NONE);
	MonsterActionEventMap[MonsterStateType::NONE][MonsterActionType::NONE]();
	//ChangeMontageAnimation(MonsterAnimationType::IDLE);

	SkeletalMeshComp = GetMesh();
	SwordMeshComp = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

	DeactivateSMOverlap();
	SwordTrailComp->Deactivate();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();

	WeaponOpacity = 0.171653f;
	MeshOpacity = 0.171653f;
	//MonsterHpWidget = Cast<UMonsterWidget>(HpWidget->GetWidget());
	
	GrabShieldCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GrabCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DeactivateHpBar();

	TargetDetectionCollison->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnTargetDetectionBeginOverlap);
	TargetDetectionCollison->OnComponentEndOverlap.AddDynamic(this, &AEnemyMonster::OnTargetDetectionEndOverlap);

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnWeaponOverlapBegin);
	WeaponOverlapStaticMeshCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnSMOverlapBegin);
	WeaponOverlapStaticMeshCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyMonster::OnSMOverlapEnd);

	ParryingCollision1->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnParryingOverlap);
	GrabCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnGrabCollisionOverlapBegin);
}


void AEnemyMonster::ChangeMontageAnimation(MonsterAnimationType type)
{
	AnimInstance->StopMontage(MontageMap[AnimationType]);
	AnimationType = type;
	StateType = AnimTypeToStateType[type];
	AnimInstance->PlayMontage(MontageMap[type]);
}

void AEnemyMonster::ChangeActionType(MonsterActionType type)
{
	ActionType = type;
}

void AEnemyMonster::DeactivateHpBar()
{
	MonsterHPWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AEnemyMonster::OnTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActionType == MonsterActionType::DEAD)return;
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
		PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::camera, true);
	}
	TracePlayer = true;
	MonsterMoveEventIndex = 1;
	TargetDetectEventMap[AttackType]();
}

void AEnemyMonster::OnTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AttackAnimationType = MonsterAnimationType::NONE;
}

void AEnemyMonster::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	if (OtherActor->TakeDamage(SkillInfoMap[AttackAnimationType].Damage, CharacterDamageEvent, nullptr, this))
	{
		HitStop();
		CameraShake(PlayerCameraShake);

		VibrateGamePad(0.4f, 0.4f);

		AObjectPool& objectpool = AObjectPool::GetInstance();
		objectpool.SpawnObject(objectpool.ObjectArray[8].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		objectpool.SpawnObject(objectpool.ObjectArray[9].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		objectpool.SpawnObject(objectpool.ObjectArray[31].ObjClass, OtherActor->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
	}	
}

void AEnemyMonster::OnSMOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SwordVFXSpawn();
}

void AEnemyMonster::OnSMOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorldTimerManager().ClearTimer(SMOverlapTimerHandler);
}

void AEnemyMonster::OnParryingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Imotal = true;

	MonsterDataStruct.CharacterHp -= MonsterDataStruct.CharacterHp;
	MonsterHPWidget->DecreaseHPGradual(this, MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp);

	//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
	PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::grogy, true);

	DeactivateSMOverlap();
	DeactivateRightWeapon();
	ParryingCollision1->Deactivate();

	ChangeMontageAnimation(MonsterAnimationType::DEAD);

	VibrateGamePad(1.0f, 0.4);
	AObjectPool& objectpool = AObjectPool::GetInstance();
	objectpool.SpawnObject(objectpool.ObjectArray[6].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	objectpool.SpawnObject(objectpool.ObjectArray[7].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	objectpool.SpawnObject(objectpool.ObjectArray[7].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	objectpool.SpawnObject(objectpool.ObjectArray[3].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
}

void AEnemyMonster::OnGrabCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->TakeDamage(10.0f, CharacterDamageEvent, nullptr, this))
	{
		//UCombatManager::GetInstance().HitMonsterInfoArray.AddUnique(Cast<ABaseCharacter>(OtherActor));

		VibrateGamePad(0.4f, 0.4f);

		AObjectPool& objectpool = AObjectPool::GetInstance();
		objectpool.SpawnObject(objectpool.ObjectArray[8].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		objectpool.SpawnObject(objectpool.ObjectArray[9].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		objectpool.SpawnObject(objectpool.ObjectArray[31].ObjClass, OtherActor->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
	}
}


void AEnemyMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	TracePlayer = false;
	if (StateType == MonsterStateType::CANTACT || GetMesh()->GetCollisionProfileName() == "Ragdoll")return;
	AttackAnimationType = AttackAnimType;
	if (ActionType != MonsterActionType::ATTACK)
	{
		MonsterController->StopMovement();
		AnimInstance->StopMontage(MontageMap[AnimationType]);
		float RandomValue = FMath::RandRange(0, 100) * 0.01f;
		if (SetActionByRandomMap.Contains(AttackAnimType))
		{
			MonsterMoveEventIndex = 1;
			SetActionByRandomMap[AttackAnimType](RandomValue);
		}
	}
}

void AEnemyMonster::EndAttackTrigger(MonsterAnimationType AttackAnimType)
{
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP)return;
	TracePlayer = true;
}

void AEnemyMonster::ShotProjectile()
{
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[4].ObjClass, GetActorLocation() + GetActorRotation().Vector() * 200.0f, GetActorRotation());
}


void AEnemyMonster::Rotate()
{
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP
		|| AnimationType == MonsterAnimationType::EXECUTION)return;
	SetActorRotation(FMath::Lerp(GetActorRotation(), YawRotation, MonsterDataStruct.RotateSpeed * fDeltaTime));
}

float AEnemyMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Imotal)
	{
		return 0;
	}
	
	//MonsterHpWidget->Hp->SetVisibility(ESlateVisibility::Visible);
	//MonsterHpWidget->HpBG->SetVisibility(ESlateVisibility::Visible);
	GetWorldTimerManager().SetTimer(HpTimer, this, &AEnemyMonster::DeactivateHpBar, 3.0f);

	DeactivateHitCollision();

	MonsterHPWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	MonsterDataStruct.CharacterHp -= DamageAmount;
	MonsterHPWidget->DecreaseHPGradual(this, MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp);


	if (MonsterDataStruct.CharacterHp <= 0)
	{
		Imotal = true;
		MonsterController->StopMovement();
		DeactivateSMOverlap();
		ParryingCollision1->Deactivate();
		DeactivateRightWeapon();
		//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
		PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::grogy, true);
		ChangeMontageAnimation(MonsterAnimationType::DEAD);
		return DamageAmount;
	}

	//if (ArmorType == EArmorType::HIGH)
	//{
	//	return DamageAmount;
	//}
	//
	//if (DamageAmount >= 30)
	//{
	//	MonsterController->StopMovement();
	//	AnimInstance->StopMontage(MontageMap[AnimationType]);
	//	if (MontageEndEventMap.Contains(AnimationType))
	//		MontageEndEventMap[AnimationType]();
	//
	//	ChangeActionType(MonsterActionType::NONE);
	//	ChangeMontageAnimation(MonsterAnimationType::HIT);
	//}
	//else if(ArmorType == EArmorType::LOW)
	//{
	//	MonsterController->StopMovement();
	//	AnimInstance->StopMontage(MontageMap[AnimationType]);
	//	if (MontageEndEventMap.Contains(AnimationType))
	//		MontageEndEventMap[AnimationType]();
	//
	//	ChangeActionType(MonsterActionType::NONE);
	//	ChangeMontageAnimation(MonsterAnimationType::HIT);
	//}
	return DamageAmount;
}

void AEnemyMonster::CheckMontageEndNotify()
{
	if (MontageEndEventMap.Contains(AnimationType))
	{
		MontageEndEventMap[AnimationType]();
	}
}

void AEnemyMonster::PlayExecutionAnimation()
{
	IsStun = false;
	CanExecution = false;
	ChangeMontageAnimation(MonsterAnimationType::EXECUTION);
}

void AEnemyMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	fDeltaTime = DeltaTime;

	CheckDIstanceMap[IsDetect]();
	MonsterTickEventMap[ActionType]();	

	Rotate();
}

void AEnemyMonster::ActivateLockOnImage(bool value)
{
	value ? MonsterLockOnWidget->SetVisibility(ESlateVisibility::HitTestInvisible) : MonsterLockOnWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AEnemyMonster::BeforeAttackNotify(bool value)
{
	if (value == true)
	{

	}
	else
	{

	}
}

void AEnemyMonster::AfterAttackNotify(bool value)
{
	if (value == true)
	{

	}
}

void AEnemyMonster::IsNotifyActive(bool value)
{
	if (NotifyBeginEndEventMap.Contains(AnimationType))
	{
		NotifyBeginEndEventMap[AnimationType][value]();
	}
}

void AEnemyMonster::RespawnCharacter()
{
	Super::RespawnCharacter();

	WeaponOpacity = 0.171653f;
	MeshOpacity = 0.171653f;

	ActivateHitCollision();
	MonsterDataStruct.CharacterHp = MonsterDataStruct.CharacterMaxHp;
	MonsterHPWidget->SetHP(1.0f);

	//Imotal = false;
	//PlayerCharacter = nullptr;
	//YawRotation = GetActorRotation();

	//SetActorHiddenInGame(false);
	//SetActorEnableCollision(true);
	//SetActorTickEnabled(true);

	//ChangeActionType(MonsterActionType::NONE);
	//ChangeMontageAnimation(MonsterAnimationType::IDLE);
}

void AEnemyMonster::ResumeMontage()
{
	if (MontageMap.Contains(AnimationType))
		AnimInstance->ResumeMontage(MontageMap[AnimationType]);
}

void AEnemyMonster::HitStop()
{
	Super::HitStop();
	if (MontageMap.Contains(AnimationType))
		AnimInstance->PauseAnimation(MontageMap[AnimationType]);
}