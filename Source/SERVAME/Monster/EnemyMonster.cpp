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

	//SwordTrailComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Sword Trail"));
	//SwordTrailComp->SetupAttachment(GetMesh(), FName("Weapon_Bone"));
	//SwordTrailComp->SetCollisionProfileName("Sword Trail");

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


	RandomRotateMap.Add(0, [](AEnemyMonster* monster)
		{
			monster->AddMovementInput(-((monster->GetActorRotation() - FRotator(0, 0, 0)).Vector() * monster->DiagonalSpeed * monster->fDeltaTime));
		});
	RandomRotateMap.Add(1, [](AEnemyMonster* monster)
		{
			monster->AddMovementInput(-((monster->GetActorRotation() - FRotator(0, -45, 0)).Vector() * monster->DiagonalSpeed * monster->fDeltaTime));
		});
	RandomRotateMap.Add(2, [](AEnemyMonster* monster)
		{
			monster->AddMovementInput(-((monster->GetActorRotation() - FRotator(0, 45, 0)).Vector() * monster->DiagonalSpeed * monster->fDeltaTime));
		});
	RandomRotateMap.Add(3, [](AEnemyMonster* monster)
		{
			
		});

	MonsterMoveMap.Add(0, [](AEnemyMonster* monster)
		{
			monster->MonsterController->Movement(monster->PatrolLocation);
		});
	MonsterMoveMap.Add(1, [](AEnemyMonster* monster)
		{			
			monster->DiagonalSpeed = monster->MonsterDataStruct.CharacterOriginSpeed;
			monster->MonsterController->Movement(monster->PlayerCharacter->GetActorLocation());
		});
	MonsterMoveMap.Add(2, [](AEnemyMonster* monster)
		{
			monster->DiagonalSpeed = monster->MonsterDataStruct.LockOnWalkSpeed * sqrt(0.5f) + monster->MonsterDataStruct.LockOnWalkSpeed * sqrt(0.5f);
			monster->RandomRotateMap[monster->MonsterRandomMove](monster);
		});


	CheckDIstanceMap.Add(false, [](AEnemyMonster* monster)
		{
			
		});
	CheckDIstanceMap.Add(true, [](AEnemyMonster* monster)
		{
			monster->CurrentDistance = FVector::Distance(monster->GetActorLocation(), monster->PlayerCharacter->GetActorLocation());
		});

	MonsterActionEventMap.Add(MonsterStateType::NONE, TMap<MonsterActionType, TFunction<void(AEnemyMonster * character)>>());
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::NONE, [](AEnemyMonster* monster)
		{
			monster->ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::DODGE, [](AEnemyMonster* monster)
		{

		});
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::ATTACK, [](AEnemyMonster* monster)
		{
			monster->ChangeActionType(MonsterActionType::ATTACK);
			monster->ChangeMontageAnimation(monster->AttackAnimationType);
		});
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::POWERATTACK, [](AEnemyMonster* monster)
		{

		});
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::MOVE, [](AEnemyMonster* monster)
		{
			monster->ChangeActionType(MonsterActionType::MOVE);
			monster->ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
		});
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::HIT, [](AEnemyMonster* monster)
		{

		});
	MonsterActionEventMap[MonsterStateType::NONE].Add(MonsterActionType::DEAD, [](AEnemyMonster* monster) {});

	MonsterActionEventMap.Add(MonsterStateType::BEFOREATTACK, TMap<MonsterActionType, TFunction<void(AEnemyMonster * character)>>());
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::NONE, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::DODGE, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::ATTACK, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::POWERATTACK, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::MOVE, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::HIT, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::BEFOREATTACK].Add(MonsterActionType::DEAD, [](AEnemyMonster* monster) {});

	MonsterActionEventMap.Add(MonsterStateType::AFTERATTACK, TMap<MonsterActionType, TFunction<void(AEnemyMonster * character)>>());
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::NONE, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::DODGE, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::ATTACK, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::POWERATTACK, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::MOVE, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::HIT, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::AFTERATTACK].Add(MonsterActionType::DEAD, [](AEnemyMonster* monster) {});

	MonsterActionEventMap.Add(MonsterStateType::CANTACT, TMap<MonsterActionType, TFunction<void(AEnemyMonster * character)>>());
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::NONE, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::DODGE, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::ATTACK, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::POWERATTACK, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::MOVE, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::HIT, [](AEnemyMonster* monster) {});
	MonsterActionEventMap[MonsterStateType::CANTACT].Add(MonsterActionType::DEAD, [](AEnemyMonster* monster) {});

	MonsterTickEventMap.Add(MonsterActionType::NONE, [](AEnemyMonster* monster)
		{
			//test
			monster->RotateMap[monster->PlayerCharacter != nullptr](monster);
		});

	MonsterTickEventMap.Add(MonsterActionType::DODGE, [](AEnemyMonster* monster)
		{

		});

	MonsterTickEventMap.Add(MonsterActionType::ATTACK, [](AEnemyMonster* monster)
		{
			monster->RotateMap[true](monster);
		});

	MonsterTickEventMap.Add(MonsterActionType::POWERATTACK, [](AEnemyMonster* monster)
		{
			monster->RotateMap[true](monster);
		});

	MonsterTickEventMap.Add(MonsterActionType::MOVE, [](AEnemyMonster* monster)
		{
			monster->RotateMap[monster->PlayerCharacter != nullptr](monster);
			monster->MonsterMoveMap[monster->MonsterMoveEventIndex](monster);
		});

	MonsterTickEventMap.Add(MonsterActionType::HIT, [](AEnemyMonster* monster)
		{

		});

	MonsterTickEventMap.Add(MonsterActionType::DEAD, [](AEnemyMonster* monster)
		{
			monster->SkeletalMeshComp->SetScalarParameterValueOnMaterials("Opacity", monster->MeshOpacity -= monster->fDeltaTime * 0.25f);
			monster->SwordMeshComp->SetScalarParameterValueOnMaterials("Opacity", monster->WeaponOpacity  -= monster->fDeltaTime * 0.25f);
			if (monster->WeaponOpacity < 0.0f)
			{
				monster->SetActorHiddenInGame(true);
				monster->SetActorEnableCollision(false);
				monster->SetActorTickEnabled(false);

				if (monster->PlayerCharacter != nullptr)
				{
					if (monster->PlayerCharacter->IsLockOn)
					{
						monster->PlayerCharacter->GetCompsInScreen(monster->PlayerCharacter->TargetCompArray);
						monster->PlayerCharacter->GetFirstTarget();
						if (monster->PlayerCharacter->TargetComp == nullptr)
							monster->PlayerCharacter->LockOn();
					}
				}
			}
		});	


	MontageEndEventMap.Add(MonsterAnimationType::DEAD, [](AEnemyMonster* monster)
		{
			monster->ChangeMontageAnimation(MonsterAnimationType::DEADLOOP);
			monster->IsStun = true;
			monster->CanExecution = true;
		});

	MontageEndEventMap.Add(MonsterAnimationType::DEADLOOP, [](AEnemyMonster* monster)
		{
			monster->ChangeMontageAnimation(MonsterAnimationType::DEADLOOP);
		});

	MontageEndEventMap.Add(MonsterAnimationType::EXECUTION, [](AEnemyMonster* monster)
		{
			monster->ChangeActionType(MonsterActionType::DEAD);
		});

	MontageEndEventMap.Add(MonsterAnimationType::STANDBY, [](AEnemyMonster* monster)
		{
			monster->ChangeMontageAnimation(MonsterAnimationType::STANDBY);
		});

	MontageEndEventMap.Add(MonsterAnimationType::IDLE, [](AEnemyMonster* monster)
		{
			if (monster->PlayerCharacter)
			{
				monster->StartAttackTrigger(monster->AttackAnimationType);
			}
			else
			{
				monster->ChangeMontageAnimation(MonsterAnimationType::IDLE);
			}
		});

	MontageEndEventMap.Add(MonsterAnimationType::ATTACK1, [](AEnemyMonster* monster)
		{			
			if (monster->TracePlayer)
			{
				monster->MonsterMoveEventIndex = 1;
				monster->ChangeActionType(MonsterActionType::MOVE);
				monster->ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
			}
			else
			{
				monster->ChangeActionType(MonsterActionType::NONE);
				monster->ChangeMontageAnimation(MonsterAnimationType::IDLE);
			}				
		});
	
	MontageEndEventMap.Add(MonsterAnimationType::POWERATTACK1, MontageEndEventMap[MonsterAnimationType::ATTACK1]);
	MontageEndEventMap.Add(MonsterAnimationType::RANGEATTACK1, MontageEndEventMap[MonsterAnimationType::ATTACK1]);
	MontageEndEventMap.Add(MonsterAnimationType::DASHATTACK1, MontageEndEventMap[MonsterAnimationType::ATTACK1]);

	MontageEndEventMap.Add(MonsterAnimationType::HIT, [](AEnemyMonster* monster)
		{
			monster->ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::IDLE, TMap<bool, TFunction<void(AEnemyMonster* monster)>>());
	NotifyBeginEndEventMap[MonsterAnimationType::IDLE].Add(true, [](AEnemyMonster* monster)
		{
			if (monster->AttackAnimationType != MonsterAnimationType::NONE)
				monster->StartAttackTrigger(monster->AttackAnimationType);

		});
	NotifyBeginEndEventMap[MonsterAnimationType::IDLE].Add(false, [](AEnemyMonster* monster)
		{
			if (monster->AttackAnimationType != MonsterAnimationType::NONE)
				monster->StartAttackTrigger(monster->AttackAnimationType);
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::ATTACK1, TMap<bool, TFunction<void(AEnemyMonster* monster)>>());
	NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1].Add(true, [](AEnemyMonster* monster)
		{
			//monster->//SwordTrailComp->Activate();
			//monster->ParryingCollision1->Activate();
			monster->ActivateSMOverlap();
			monster->ActivateRightWeapon();
			AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[24].ObjClass, monster->GetActorLocation(), FRotator::ZeroRotator);

		});
	NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1].Add(false, [](AEnemyMonster* monster)
		{
			monster->DeactivateRightWeapon();
			monster->ParryingCollision1->Deactivate();
			//monster->//SwordTrailComp->Deactivate();
//			monster->DeactivateSMOverlap();
		});	

	NotifyBeginEndEventMap.Add(MonsterAnimationType::POWERATTACK1, TMap<bool, TFunction<void(AEnemyMonster* monster)>>());
	NotifyBeginEndEventMap[MonsterAnimationType::POWERATTACK1].Add(true,  NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[MonsterAnimationType::POWERATTACK1].Add(false, NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(MonsterAnimationType::DASHATTACK1, TMap<bool, TFunction<void(AEnemyMonster* monster)>>());
	NotifyBeginEndEventMap[MonsterAnimationType::DASHATTACK1].Add(true, NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[MonsterAnimationType::DASHATTACK1].Add(false, NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(MonsterAnimationType::RANGEATTACK1, TMap<bool, TFunction<void(AEnemyMonster* monster)>>());
	NotifyBeginEndEventMap[MonsterAnimationType::RANGEATTACK1].Add(true, [](AEnemyMonster* monster)
		{
			monster->NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][true](monster);
			monster->ShotProjectile();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::RANGEATTACK1].Add(false, [](AEnemyMonster* monster)
		{
			monster->NotifyBeginEndEventMap[MonsterAnimationType::ATTACK1][false](monster);
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::EXECUTION, TMap<bool, TFunction<void(AEnemyMonster* monster)>>());
	NotifyBeginEndEventMap[MonsterAnimationType::EXECUTION].Add(true, [](AEnemyMonster* monster)
		{
		});
	NotifyBeginEndEventMap[MonsterAnimationType::EXECUTION].Add(false, [](AEnemyMonster* monster)
		{
		});

	RotateMap.Add(false, [](AEnemyMonster* monster)
		{

		});

	RotateMap.Add(true, [](AEnemyMonster* monster)
		{
			if (monster->PlayerCharacter != nullptr)
			{
				monster->TargetRotation = (monster->PlayerCharacter->GetActorLocation() - monster->GetActorLocation()).Rotation();
				monster->YawRotation = monster->TargetRotation;
			}			
		});

	TargetDetectEventMap.Add(MonsterAttackType::MELEE, [](AEnemyMonster* monster)
		{
			monster->MonsterActionEventMap[MonsterStateType::NONE][MonsterActionType::MOVE](monster);
		});
	TargetDetectEventMap.Add(MonsterAttackType::RANGE, [](AEnemyMonster* monster)
		{
			monster->MonsterActionEventMap[MonsterStateType::NONE][MonsterActionType::MOVE](monster);
		});

	SetActionByRandomMap.Add(MonsterAnimationType::ATTACK1, [](AEnemyMonster* monster, float percent)
		{
			if (percent >= 0.5)
			{

				//UGameplayStatics::SetGlobalTimeDilation(monster, 0.1f);
				monster->PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::dodge, true);
				monster->ChangeActionType(MonsterActionType::ATTACK);
				monster->ChangeMontageAnimation(MonsterAnimationType::ATTACK1);
			}
			else if (percent < 0.5f)
			{
				//UGameplayStatics::SetGlobalTimeDilation(monster, 0.1f);
				monster->PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::parrying, true);
				monster->ChangeActionType(MonsterActionType::ATTACK);
				monster->ChangeMontageAnimation(MonsterAnimationType::POWERATTACK1);
			}
		});
	SetActionByRandomMap.Add(MonsterAnimationType::DASHATTACK1, [](AEnemyMonster* monster, float percent)
		{
			if (percent <= 0.5f)
			{
				monster->ChangeActionType(MonsterActionType::ATTACK);
				monster->ChangeMontageAnimation(MonsterAnimationType::DASHATTACK1);
			}
			else
			{
				monster->ChangeActionType(MonsterActionType::MOVE);
				monster->ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
			}
		});
	SetActionByRandomMap.Add(MonsterAnimationType::RANGEATTACK1, [](AEnemyMonster* monster, float percent)
		{
			if (percent <= 0.8f)
			{
				monster->ChangeActionType(MonsterActionType::ATTACK);
				monster->ChangeMontageAnimation(MonsterAnimationType::RANGEATTACK1);
			}
			else
			{
				monster->ChangeActionType(MonsterActionType::NONE);
				monster->ChangeMontageAnimation(MonsterAnimationType::IDLE);
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

	UE_LOG(LogTemp, Warning, TEXT("monsterbegin"));
	UCombatManager::GetInstance().MonsterInfoArray.Add(this);
		
	PlayerCharacter = nullptr;
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
	

	AnimInstance = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());

	AnimationType = MonsterAnimationType::IDLE;
	ChangeActionType(MonsterActionType::NONE);
	MonsterActionEventMap[MonsterStateType::NONE][MonsterActionType::NONE](this);
	//ChangeMontageAnimation(MonsterAnimationType::IDLE);

	SkeletalMeshComp = GetMesh();
	SwordMeshComp = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

	DeactivateSMOverlap();
	//SwordTrailComp->Deactivate();
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
	UE_LOG(LogTemp, Warning, TEXT("Detect"));
	if (ActionType == MonsterActionType::DEAD)return;
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
		PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::camera, true);
	}

	MonsterMoveEventIndex = 1;
	TargetDetectEventMap[AttackType](this);
}

void AEnemyMonster::OnTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AttackAnimationType = MonsterAnimationType::NONE;
}

void AEnemyMonster::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	//if (OtherActor->TakeDamage(SkillInfoMap[AttackAnimationType].Damage, //CharacterDamageEvent, nullptr, this))
	{
		HitStop();
		CameraShake(PlayerCameraShake);

		VibrateGamePad(0.4f, 0.4f);

		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[8].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[9].ObjClass, OtherComp->GetComponentLocation(), FRotator::ZeroRotator);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[31].ObjClass, OtherActor->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
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
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[6].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[7].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[7].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[3].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator(90, 180, 0));
}

void AEnemyMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	TracePlayer = false;
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP)return;
	AttackAnimationType = AttackAnimType;
	if (ActionType != MonsterActionType::ATTACK)
	{
		MonsterController->StopMovement();
		AnimInstance->StopMontage(MontageMap[AnimationType]);
		float RandomValue = FMath::RandRange(0, 100) * 0.01f;
		if (SetActionByRandomMap.Contains(AttackAnimType))
		{
			MonsterMoveEventIndex = 1;
			SetActionByRandomMap[AttackAnimType](this, RandomValue);
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

//float AEnemyMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//
//	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//	if (Imotal)return 0;
//
//	
//	//MonsterHpWidget->Hp->SetVisibility(ESlateVisibility::Visible);
//	//MonsterHpWidget->HpBG->SetVisibility(ESlateVisibility::Visible);
//	GetWorldTimerManager().SetTimer(HpTimer, this, &AEnemyMonster::DeactivateHpBar, 3.0f);
//
//	DeactivateHitCollision();
//
//	MonsterHPWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
//	MonsterDataStruct.CharacterHp -= DamageAmount;
//	MonsterHPWidget->DecreaseHPGradual(this, MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp);
//
//
//	if (MonsterDataStruct.CharacterHp <= 0)
//	{
//		Imotal = true;
//		MonsterController->StopMovement();
//		DeactivateSMOverlap();
//		ParryingCollision1->Deactivate();
//		DeactivateRightWeapon();
//		//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
//		PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::grogy, true);
//		ChangeMontageAnimation(MonsterAnimationType::DEAD);
//		return DamageAmount;
//	}
//
//	if (ArmorType == EArmorType::HIGH)
//	{
//		return DamageAmount;
//	}
//
//	if (DamageAmount >= 30)
//	{
//		MonsterController->StopMovement();
//		AnimInstance->StopMontage(MontageMap[AnimationType]);
//		if (MontageEndEventMap.Contains(AnimationType))
//			MontageEndEventMap[AnimationType](this);
//
//		ChangeActionType(MonsterActionType::NONE);
//		ChangeMontageAnimation(MonsterAnimationType::HIT);
//	}
//	else if(ArmorType == EArmorType::LOW)
//	{
//		MonsterController->StopMovement();
//		AnimInstance->StopMontage(MontageMap[AnimationType]);
//		if (MontageEndEventMap.Contains(AnimationType))
//			MontageEndEventMap[AnimationType](this);
//
//		ChangeActionType(MonsterActionType::NONE);
//		ChangeMontageAnimation(MonsterAnimationType::HIT);
//	}
//	return DamageAmount;
//}

void AEnemyMonster::CheckMontageEndNotify()
{
	if (MontageEndEventMap.Contains(AnimationType))
	{
		MontageEndEventMap[AnimationType](this);
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

	
	CheckDIstanceMap[IsDetect](this);
	MonsterTickEventMap[ActionType](this);	
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
		NotifyBeginEndEventMap[AnimationType][value](this);
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