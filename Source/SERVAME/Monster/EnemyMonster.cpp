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
	AnimTypeToStateType.Add(MonsterAnimationType::ELITEKNIGHT_STAND_IDLE, MonsterStateType::NONE);

	AnimTypeToStateType.Add(MonsterAnimationType::ATTACK1, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::POWERATTACK1, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::RANGEATTACK1, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::DASHATTACK1, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::STINGaTTACK1, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::SPRINTATTACK, MonsterStateType::BEFOREATTACK);

	AnimTypeToStateType.Add(MonsterAnimationType::HIT, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::SUPER_HIT, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::BACKHIT, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::DEAD, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::DEADLOOP, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::EXECUTION, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::SPAWNING, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::PARRYING, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::GROGGY_START, MonsterStateType::CANTACT); 
	AnimTypeToStateType.Add(MonsterAnimationType::GROGGY_DEAD, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::GROGGY_LOOP, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::JAMSIG_SIT_IDLE, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::ELITEKNIGHT_START, MonsterStateType::CANTACT);
	AnimTypeToStateType.Add(MonsterAnimationType::JAMSIG_STANDUP, MonsterStateType::CANTACT);

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
			if (PlayerCharacter != nullptr)
				CurrentDistance = FVector::Distance(GetActorLocation(), PlayerCharacter->GetActorLocation());
		});

	MonsterTickEventMap.Add(MonsterActionType::NONE, [&]()
		{
			//test
			if (MonsterController->FindPlayer)
				RotateMap[PlayerCharacter != nullptr]();
		});

	MonsterTickEventMap.Add(MonsterActionType::DODGE, [&]()
		{

		});

	MonsterTickEventMap.Add(MonsterActionType::ATTACK, [&]()
		{
			if (MonsterController->FindPlayer == false)
				MonsterController->FindPlayer = true;

			RotateMap[true]();
		});

	MonsterTickEventMap.Add(MonsterActionType::POWERATTACK, [&]()
		{
			RotateMap[true]();
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
				RotateMap[PlayerCharacter != nullptr]();
				MonsterMoveMap[MonsterMoveEventIndex]();
			}
		});

	MonsterTickEventMap.Add(MonsterActionType::HIT, [&]()
		{

		});

	MonsterTickEventMap.Add(MonsterActionType::RUN, [&]()
		{

		});

	MonsterTickEventMap.Add(MonsterActionType::DEAD, [&]()
		{	
			if (MyMonsterType == MonsterType::TUTORIAL)
			{
				MeshOpacity -= fDeltaTime * 0.25f;
				WeaponOpacity -= fDeltaTime * 0.25f;

				SkeletalMeshComp->SetScalarParameterValueOnMaterials("Opacity", MeshOpacity);
				SwordMeshComp->SetScalarParameterValueOnMaterials("Opacity", WeaponOpacity);
			}

			if (MeshOpacity <= 0.0f)
			{
				SetActive(false);
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

						//if (MyMonsterType == MonsterType::TUTORIAL && PlayerCharacter->IsAlive())
						//	PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::soul, true);

						if (PlayerCharacter->TargetComp == nullptr)
						{
							PlayerCharacter->LockOn();
						}
						else
						{
							Cast<ABaseCharacter>(PlayerCharacter->TargetComp->GetOwner())->ActivateLockOnImage(true, PlayerCharacter->TargetComp);
						}
					}
				}
			}
		});	

	MontageEndEventMap.Add(MonsterAnimationType::PARRYING, [&]()
		{
			ActivateRightWeapon();

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

	MontageEndEventMap.Add(MonsterAnimationType::DEAD, [&]()
		{
			ChangeMontageAnimation(MonsterAnimationType::DEADLOOP);
			IsStun = true;
			//CanExecution = true;
		});

	MontageEndEventMap.Add(MonsterAnimationType::DEADLOOP, [&]()
		{
			ChangeMontageAnimation(MonsterAnimationType::DEADLOOP);
		});

	MontageEndEventMap.Add(MonsterAnimationType::EXECUTION, [&]()
		{
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
			//ChangeActionType(MonsterActionType::DEAD);
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

	MontageEndEventMap.Add(MonsterAnimationType::FORWARDMOVE, [=]()
		{
			if (TracePlayer)
			{
				MonsterMoveEventIndex = 1;
				ChangeActionType(MonsterActionType::MOVE);
				ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
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
	MontageEndEventMap.Add(MonsterAnimationType::STINGaTTACK1, MontageEndEventMap[MonsterAnimationType::ATTACK1]);
	MontageEndEventMap.Add(MonsterAnimationType::SPRINTATTACK, MontageEndEventMap[MonsterAnimationType::ATTACK1]);

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

	MontageEndEventMap.Add(MonsterAnimationType::BACKHIT, [&]()
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
			ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			ActivateSMOverlap();
			ActivateRightWeapon();
			AObjectPool& objectpool = AObjectPool::GetInstance();
			objectpool.SpawnObject(objectpool.ObjectArray[24].ObjClass, GetActorLocation(), FRotator::ZeroRotator);

		});
	NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1].Add(false, [&]()
		{
			DeactivateRightWeapon();
			ParryingCollision1->Deactivate();
			ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DeactivateSMOverlap();
		});	

	NotifyBeginEndEventMap.Add(MonsterAnimationType::POWERATTACK1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::POWERATTACK1].Add(true,  NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[MonsterAnimationType::POWERATTACK1].Add(false, NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(MonsterAnimationType::DASHATTACK1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::DASHATTACK1].Add(true, NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[MonsterAnimationType::DASHATTACK1].Add(false, NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(MonsterAnimationType::STINGaTTACK1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::STINGaTTACK1].Add(true, NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[MonsterAnimationType::STINGaTTACK1].Add(false, NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(MonsterAnimationType::SPRINTATTACK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::SPRINTATTACK].Add(true, NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[MonsterAnimationType::SPRINTATTACK].Add(false, NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][false]);


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
			if (percent >= 0.5f)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::ATTACK1);

				//if (MyMonsterType == MonsterType::TUTORIAL && !asd2 && PlayerCharacter->IsAlive())
				//{
				//	PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::dodge, true);
				//	asd2 = true;
				//	return;
				//}
				//else if (MyMonsterType == MonsterType::TUTORIAL && PlayerCharacter->IsAlive())
				//{
				//	PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::shield, true);
				//}
			}
			else if (percent < 0.5f)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::POWERATTACK1);

				//if (MyMonsterType == MonsterType::TUTORIAL && !asd && PlayerCharacter->IsAlive())
				//{
				//	PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::parrying, true);
				//	asd = true;
				//	return;
				//}
				//else if (MyMonsterType == MonsterType::TUTORIAL && PlayerCharacter->IsAlive())
				//{
				//	PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::shieldattack, true);
				//}
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

	TArray<UActorComponent*>ActorCompArray;
	ActorCompArray = GetComponentsByTag(USphereComponent::StaticClass(), FName("LockOnTarget"));
	LockOnComp = Cast<USphereComponent>(ActorCompArray[0]);

	PlayerCharacter = nullptr;
	
	GetCharacterMovement()->bOrientRotationToMovement = false;

	AnimInstance = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());
	
	if (MyMonsterType != MonsterType::DEADBODYOFKNIGHT)
	{
		AnimationType = MonsterAnimationType::IDLE;
		ChangeActionType(MonsterActionType::NONE);
		ChangeMontageAnimation(MonsterAnimationType::IDLE);
	}

	SkeletalMeshComp = GetMesh();
	SwordMeshComp = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

	DeactivateSMOverlap();
	SwordTrailComp->Deactivate();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();

	WeaponOpacity = 0.171653f;
	MeshOpacity = 0.171653f;
	//MonsterHpWidget = Cast<UMonsterWidget>(HpWidget->GetWidget());
	
	TargetDetectionCollison->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnTargetDetectionBeginOverlap);
	TargetDetectionCollison->OnComponentEndOverlap.AddDynamic(this, &AEnemyMonster::OnTargetDetectionEndOverlap);

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnWeaponOverlapBegin);
	WeaponOverlapStaticMeshCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnSMOverlapBegin);
	WeaponOverlapStaticMeshCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyMonster::OnSMOverlapEnd);

	ParryingCollision1->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnParryingOverlap);

	UCombatManager::GetInstance().AddMonsterInfo(this);
}

void AEnemyMonster::ChangeMontageAnimation(MonsterAnimationType type)
{
	DeactivateRightWeapon();
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

void AEnemyMonster::TickOverlap()
{
	//if (PlayerCharacter == nullptr && otherActor != nullptr)
	//{
	//	PlayerCharacter = Cast<APlayerCharacter>(otherActor);
	//	//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
	//	if (MyMonsterType == MonsterType::TUTORIAL && PlayerCharacter->IsAlive())
	//		PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::camera, true);
	//}

	if (!MonsterController->FindPlayer)
		return;

	IsOverlap = false;
	IsDetect = true;

	if (ActionType == MonsterActionType::DEAD)
		return;
	if (ActionType == MonsterActionType::ATTACK)
		return;

	TracePlayer = true;
	MonsterMoveEventIndex = 1;

	TargetDetectEventMap[AttackType]();
}

void AEnemyMonster::MonsterLog(int id, FString str)
{
	if(MonsterID == id)
		UE_LOG(LogTemp, Warning, TEXT("%s"), *str);
}

void AEnemyMonster::MonsterLog(int id, float f)
{
	if (MonsterID == id)
		UE_LOG(LogTemp, Warning, TEXT("%f"), f);
}

void AEnemyMonster::MonsterLog(int id, int i)
{
	if (MonsterID == id)
		UE_LOG(LogTemp, Warning, TEXT("%d"), i);
}


void AEnemyMonster::PlayMonsterSoundInPool(EMonsterAudioType AudioType)
{
	AObjectPool& objectpool = AObjectPool::GetInstance();

	auto Obj = objectpool.SpawnObject(objectpool.ObjectArray[MONSTERSOUNDOP].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
	auto MonsterSound = Cast<AMonsterSoundObjectInpool>(Obj);

	MonsterSound->PlayMonsterSound(AudioType);
}

void AEnemyMonster::OnTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	otherActor = OtherActor;
	IsOverlap = true;
}

void AEnemyMonster::OnTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MyMonsterType == MonsterType::NUN || MyMonsterType == MonsterType::ILLUSION_NUN)
	{
		return;
	}

	IsOverlap = false;
	AttackAnimationType = MonsterAnimationType::NONE;
}

void AEnemyMonster::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == PlayerCharacter)
	{
		ParryingCollision1->Deactivate();
		DeactivateRightWeapon();

		AObjectPool& objectpool = AObjectPool::GetInstance();
		if (OtherComp->GetName() == "ShieldCollision")
		{
			if(SkillInfoMap.Contains(AttackAnimationType))
				PlayerCharacter->SetShieldHP(-SkillInfoMap[AttackAnimationType].Damage, GetActorLocation());
			CameraShake(PlayerCameraShake);
			VibrateGamePad(0.4f, 0.4f);
			objectpool.SpawnObject(objectpool.ObjectArray[6].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
			//objectpool.SpawnObject(objectpool.ObjectArray[9].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
			objectpool.SpawnObject(objectpool.ObjectArray[19].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
			return;
		}
		if (SkillInfoMap.Contains(AttackAnimationType))
		{
			//if (OtherActor->TakeDamage(PlayerDataStruct.BaseDamage * PlayerDataStruct.DamageList[AnimInstance->PlayerAnimationType].Damage, CharacterDamageEvent, nullptr, this))
			if (OtherActor->TakeDamage(SkillInfoMap[AttackAnimationType].Damage, CharacterDamageEvent, nullptr, this))
			{
				if (!IsStun)
					HitStop();

				CameraShake(PlayerCameraShake);

				VibrateGamePad(0.4f, 0.4f);

				objectpool.SpawnObject(objectpool.ObjectArray[6].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
				objectpool.SpawnObject(objectpool.ObjectArray[31].ObjClass, OtherActor->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
				objectpool.SpawnObject(objectpool.ObjectArray[5].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator::ZeroRotator);
				//objectpool.SpawnObject(objectpool.ObjectArray[9].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
			}
		}
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
	//MonsterDataStruct.CharacterHp -= MonsterDataStruct.CharacterHp;
	//MonsterHPWidget->DecreaseHPGradual(this, MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp);

	//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
	//if (MyMonsterType == MonsterType::TUTORIAL && PlayerCharacter->IsAlive())
	//	PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::grogy, true);

	ParryingStun();

	VibrateGamePad(1.0f, 0.4);
	AObjectPool& objectpool = AObjectPool::GetInstance();
	objectpool.SpawnObject(objectpool.ObjectArray[6].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	objectpool.SpawnObject(objectpool.ObjectArray[7].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	objectpool.SpawnObject(objectpool.ObjectArray[7].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	objectpool.SpawnObject(objectpool.ObjectArray[3].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
}

int AEnemyMonster::GetRandNum(int Min, int Max)
{
	std::srand(time(NULL));
	auto Val = rand() % Max + Min;
	return Val;
}

void AEnemyMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	TracePlayer = false;
	if (StateType == MonsterStateType::CANTACT)
		return;
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
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP)
		return;
	TracePlayer = true;
}

void AEnemyMonster::ShotProjectile()
{
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[4].ObjClass, GetActorLocation() + GetActorRotation().Vector() * 200.0f, GetActorRotation());
}

void AEnemyMonster::Rotate()
{
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP
		/*|| AnimationType == MonsterAnimationType::EXECUTION*/)return;

	auto Rot = FRotator(0.f, GetActorRotation().Yaw, GetActorRotation().Roll);

	SetActorRotation(FMath::Lerp(Rot, YawRotation, MonsterDataStruct.RotateSpeed * fDeltaTime));
}

void AEnemyMonster::ParryingStun()
{
	//CanExecution = true;
	AnimInstance->StopMontage(MontageMap[AnimationType]);
	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	ChangeMontageAnimation(MonsterAnimationType::PARRYING);
}

void AEnemyMonster::Stun()
{
	AnimInstance->StopMontage(MontageMap[AnimationType]);
	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	ChangeMontageAnimation(MonsterAnimationType::GROGGY_START);
}

float AEnemyMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser))
		return 0.0f;

	LOG(Warning, TEXT("Monster Take Damage"));

	if (Imotal)
	{
		return 0;
	}

	DeactivateHitCollision();

	if (!IsBoss)
	{
		ActivateHpBar();
		GetWorldTimerManager().SetTimer(HpTimer, this, &AEnemyMonster::DeactivateHpBar, 3.0f);

		MonsterHPWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		MonsterDataStruct.CharacterHp -= DamageAmount;

		float CurrentPercent = MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp;
		MonsterHPWidget->DecreaseHPGradual(this, CurrentPercent);
	}

	if (MonsterDataStruct.CharacterHp <= 0)
	{
		IsDie = true;
		Die(DamageAmount);
	}

	return DamageAmount;
}

float AEnemyMonster::Die(float Dm)
{
	if (MonsterDataStruct.CharacterHp <= 0)
	{
		AObjectPool& objectpool = AObjectPool::GetInstance();
		for (int32 i = 0; i < MonsterDataStruct.DropSoulCount; i++)
		{
			float x = FMath::RandRange(-300.0f, 300.0f);
			float y = FMath::RandRange(-300.0f, 300.0f);
			float z = FMath::RandRange(-300.0f, 300.0f);

			FVector location = GetActorLocation() + FVector(x * 0.1f, y * 0.1f, z * 0.1f);
			FRotator rotation = GetActorRotation() + FRotator(x, y, z);

			objectpool.SpawnObject(objectpool.ObjectArray[36].ObjClass, location, rotation);
		}

		DeactivateHitCollision();
		Imotal = true;
		ChangeActionType(MonsterActionType::DEAD);
		StateType = MonsterStateType::CANTACT;
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

	if (!IsPatrol && CanRotate)
	{
		Rotate();
	}

	if (MonsterDataStruct.CharacterHp <= 0)
	{
		IsDie = true;
		ChangeActionType(MonsterActionType::DEAD);
		StateType = MonsterStateType::CANTACT;

		if(OpenDoor != nullptr)
		OpenDoor->BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	}

	if (IsOverlap)
		TickOverlap();
}

void AEnemyMonster::ActivateLockOnImage(bool value, UPrimitiveComponent* comp)
{
	value ? MonsterLockOnWidget->SetVisibility(ESlateVisibility::HitTestInvisible) : MonsterLockOnWidget->SetVisibility(ESlateVisibility::Collapsed);
}

bool AEnemyMonster::IsAlive()
{
	if (MonsterDataStruct.CharacterHp > 0)
		return true;
	return false;
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
	SkeletalMeshComp->SetScalarParameterValueOnMaterials("Opacity", MeshOpacity);
	//SwordMeshComp->SetScalarParameterValueOnMaterials("Opacity", WeaponOpacity);

	ActivateHitCollision();
	MonsterDataStruct.CharacterHp = MonsterDataStruct.CharacterMaxHp;
	MonsterHPWidget->SetHP(1.0f);

	Imotal = false;
	IsStun = false;
	CanRotate = true;
	//PlayerCharacter = nullptr;
	YawRotation = GetActorRotation();

	DeactivateHpBar();
	ActivateLockOnImage(false, nullptr);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
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
