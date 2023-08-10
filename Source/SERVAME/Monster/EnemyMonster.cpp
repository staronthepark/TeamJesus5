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
						SetActorTickEnabled(false);
						//PlayerCharacter->TargetCompArray.Remove(this);
						PlayerCharacter->TargetComp = nullptr;
						PlayerCharacter->GetCompsInScreen(PlayerCharacter->TargetCompArray);
						PlayerCharacter->GetFirstTarget();

						AObjectPool& objectpool =  AObjectPool::GetInstance();
						for (int32 i = 0; i < MonsterDataStruct.DropSoulCount; i++)
						{
							float x = FMath::RandRange(-30.0f, 30.0f);
							float y = FMath::RandRange(-30.0f, 30.0f);
							float z = FMath::RandRange(-30.0f, 30.0f);

							FVector location = GetActorLocation() + FVector(x, y, z);

							objectpool.SpawnObject(objectpool.ObjectArray[36].ObjClass, location, FRotator::ZeroRotator);
						}

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
			ChangeActionType(MonsterActionType::MOVE);
			ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
		});
	TargetDetectEventMap.Add(MonsterAttackType::RANGE, [&]()
		{
			ChangeActionType(MonsterActionType::MOVE);
			ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
		});

	SetActionByRandomMap.Add(MonsterAnimationType::ATTACK1, [&](float percent)
		{
			if (percent >= 0.5)
			{
				if (MyMonsterType == MonsterType::TUTORIAL)
					PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::dodge, true);
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::ATTACK1);
			}
			else if (percent < 0.5f)
			{
				//UGameplayStatics::SetGlobalTimeDilation(monster, 0.1f);
				if (MyMonsterType == MonsterType::TUTORIAL)
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

	UCombatManager::GetInstance().MonsterInfoArray.AddUnique(this);

	//test
	UCombatManager::GetInstance().HitMonsterInfoArray.AddUnique(this);

	TArray<UActorComponent*>ActorCompArray;
	ActorCompArray = GetComponentsByTag(USphereComponent::StaticClass(), FName("LockOnTarget"));
	LockOnComp = Cast<USphereComponent>(ActorCompArray[0]);

		
	PlayerCharacter = nullptr;
	
	GetCharacterMovement()->bOrientRotationToMovement = false;

	AnimInstance = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());

	AnimationType = MonsterAnimationType::IDLE;
	ChangeActionType(MonsterActionType::NONE);
	ChangeMontageAnimation(MonsterAnimationType::IDLE);

	SkeletalMeshComp = GetMesh();
	SwordMeshComp = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

	DeactivateSMOverlap();
	SwordTrailComp->Deactivate();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();

	WeaponOpacity = 0.171653f;
	MeshOpacity = 0.171653f;
	//MonsterHpWidget = Cast<UMonsterWidget>(HpWidget->GetWidget());
	
	DeactivateHpBar();

	TargetDetectionCollison->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnTargetDetectionBeginOverlap);
	TargetDetectionCollison->OnComponentEndOverlap.AddDynamic(this, &AEnemyMonster::OnTargetDetectionEndOverlap);

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnWeaponOverlapBegin);
	WeaponOverlapStaticMeshCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnSMOverlapBegin);
	WeaponOverlapStaticMeshCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyMonster::OnSMOverlapEnd);

	ParryingCollision1->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnParryingOverlap);

	SetActive(false);
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

void AEnemyMonster::ActivateHpBar()
{
	MonsterHPWidget->SetVisibility(ESlateVisibility::Visible);
}

void AEnemyMonster::OnTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActionType == MonsterActionType::DEAD)return;
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
		if (MyMonsterType == MonsterType::TUTORIAL)
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

	AObjectPool& objectpool = AObjectPool::GetInstance();
	if (OtherComp->GetName() == "ShieldCollision")
	{
		PlayerCharacter->SetShieldHP(-SkillInfoMap[AttackAnimationType].Damage);
		CameraShake(PlayerCameraShake);
		VibrateGamePad(0.4f, 0.4f);
		objectpool.SpawnObject(objectpool.ObjectArray[8].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		objectpool.SpawnObject(objectpool.ObjectArray[9].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		return;
	}
	if (OtherActor->TakeDamage(SkillInfoMap[AttackAnimationType].Damage, CharacterDamageEvent, nullptr, this))
	{
		if(!IsStun)
			HitStop();

		CameraShake(PlayerCameraShake);

		VibrateGamePad(0.4f, 0.4f);

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
	if (MyMonsterType == MonsterType::TUTORIAL)
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

void AEnemyMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	TracePlayer = false;
	if (StateType == MonsterStateType::CANTACT)return;
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

void AEnemyMonster::Stun()
{
	AnimInstance->StopMontage(MontageMap[AnimationType]);
	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	ChangeMontageAnimation(MonsterAnimationType::DEAD);
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

	UE_LOG(LogTemp, Warning, TEXT("%f,%f"), DamageAmount, MonsterDataStruct.CharacterHp);

	Die(DamageAmount);

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

float AEnemyMonster::Die(float Dm)
{
	if (MonsterDataStruct.CharacterHp <= 0)
	{
		Imotal = true;
		//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
		//ChangeMontageAnimation(MonsterAnimationType::DEAD);
		AnimInstance->StopMontage(MontageMap[AnimationType]);
		ChangeActionType(MonsterActionType::DEAD);
		StateType = MonsterStateType::CANTACT;
		//PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::grogy, true);
		return Dm;
	}

	return Dm;
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

void AEnemyMonster::MonsterHitStop()
{
	if (MontageMap.Contains(AnimationType))
		AnimInstance->PauseAnimation(MontageMap[AnimationType]);
}

void AEnemyMonster::HitStop()
{
	Super::HitStop();

	MonsterHitStop();
}
