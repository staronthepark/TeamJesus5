#include "Jamsig.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RandomStream.h"
#include "JamsigAttackTriggerComp.h"
#include "..\..\Manager\CombatManager.h"
#include "..\..\ObjectPool\EffectObjectInPool.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "..\..\SERVAME.h"

AJamsig::AJamsig()
{
	AttackTrigger = CreateDefaultSubobject<UJamsigAttackTriggerComp>(TEXT("AttackTriggerCollision"));
	AttackTrigger->SetupAttachment(GetMesh());

	NotifyBeginEndEventMap.Add(MonsterAnimationType::IDLE, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::IDLE].Add(true, [&]()
		{
			if (TracePlayer && MonsterController->FindPlayer)
			{
				StateType = AnimTypeToStateType[MonsterAnimationType::IDLE];
				MonsterMoveEventIndex = 1;
				ChangeActionType(MonsterActionType::MOVE);
				ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
				return;
			}

			if (AttackAnimationType != MonsterAnimationType::NONE && MonsterController->FindPlayer)
				StartAttackTrigger(AttackAnimationType);
		});
	NotifyBeginEndEventMap[MonsterAnimationType::IDLE].Add(false, [&]()
		{
			if (CurrentDistance < AttackRange)
			{
				StartAttackTrigger(AttackAnimationType);
			}

			if (TracePlayer && MonsterController->FindPlayer)
			{
				StateType = AnimTypeToStateType[MonsterAnimationType::IDLE];
				MonsterMoveEventIndex = 1;
				ChangeActionType(MonsterActionType::MOVE);
				ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
				return;
			}

			if (AttackAnimationType != MonsterAnimationType::NONE && MonsterController->FindPlayer)
				StartAttackTrigger(AttackAnimationType);
		});

	MonsterMoveMap.Add(0, [&]()
		{
			MonsterController->MoveToStartLoc(SpawnLocation);
		});

	MontageEndEventMap.Add(MonsterAnimationType::FORWARDMOVE, [=]()
		{
			if (TracePlayer)
			{
				MonsterMoveEventIndex = 1;
				ChangeActionType(MonsterActionType::MOVE);
				ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
			}
			else
			{
				MonsterMoveEventIndex = 0;
				ChangeActionType(MonsterActionType::MOVE);
				ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
			}
		});

	MontageEndEventMap.Add(MonsterAnimationType::IDLE, [&]()
		{
			if (PlayerCharacter && !TracePlayer)
			{
				StartAttackTrigger(AttackAnimationType);
			}
			else if (TracePlayer)
			{
				MonsterMoveEventIndex = 1;
				ChangeActionType(MonsterActionType::MOVE);
				ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
			}
		});

	MontageEndEventMap.Add(MonsterAnimationType::JAMSIG_SIT_IDLE, [=]()
		{
			ChangeMontageAnimation(MonsterAnimationType::JAMSIG_SIT_IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::JAMSIG_STANDUP, [&]()
		{
			MonsterController->FindPlayer = true;
			CanRotate = true;
			TracePlayer = true;

			MonsterMoveEventIndex = 1;
			ChangeActionType(MonsterActionType::MOVE);
			ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::DEAD, [&]()
		{
			JamsigAnimInstance->PauseAnimation(MontageMap[AnimationType]);
		});

	MontageEndEventMap.Add(MonsterAnimationType::HIT, [&]()
		{
			MonsterController->FindPlayer = true;
			CanRotate = true;
			TracePlayer = true;

			MonsterMoveEventIndex = 1;
			ChangeActionType(MonsterActionType::MOVE);
			ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);
		});

	MonsterTickEventMap.Add(MonsterActionType::MOVE, [&]()
		{
			RotateMap[PlayerCharacter != nullptr]();
			MonsterMoveMap[MonsterMoveEventIndex]();
		});

	SetActionByRandomMap.Add(MonsterAnimationType::ATTACK1, [&](float percent)
		{
			if (percent <= 0.7f)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::ATTACK1);
			}
			else
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::POWERATTACK1);
			}
		});
}

void AJamsig::BeginPlay()
{
	Super::BeginPlay();

	CanHit = true;

	MonsterController->CanPerception = true;

	DeactivateHpBar();

	SetActive(true);
	Player = nullptr;

	JamsigAnimInstance = Cast<UJamsigAniminstance>(GetMesh()->GetAnimInstance());

	if (JamsigAnimInstance != nullptr)
	{
		JamsigAnimInstance->KnockBackStart.AddUObject(this, &AJamsig::KnockBackStart);
		//JamsigAnimInstance->KnockBackEnd.AddUObject(this, &AJamsig::KnockBackEmd);
	}

	TargetDetectionCollison->OnComponentBeginOverlap.AddDynamic(this, &AJamsig::OnJamsigTargetDetectionBeginOverlap);
	TargetDetectionCollison->OnComponentEndOverlap.AddDynamic(this, &AJamsig::OnJamsigTargetDetectionEndOverlap);

	if (SitJamsig)
	{
		ChangeMontageAnimation(MonsterAnimationType::JAMSIG_SIT_IDLE);		
	}
}

void AJamsig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MinusOpacity)
	{
		OpactiyDeltaTime += 0.005;
		SkeletalMeshComp->SetScalarParameterValueOnMaterials("Dither", MeshOpacity -= OpactiyDeltaTime);
	}
}

void AJamsig::ActivateAttackTrigger()
{
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AJamsig::DeactivateAttackTrigger()
{
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AJamsig::KnockBackStart()
{
	UE_LOG(LogTemp, Warning, TEXT("knockbackstart"));

	GetWorld()->GetTimerManager().SetTimer(KnockBackTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			IsKnockBack = false;
			GetWorld()->GetTimerManager().ClearTimer(KnockBackTimerHandle);
		}), KnockBackTime, false);

	GetWorld()->GetTimerManager().SetTimer(KnockBackDelayTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			ActivateAttackTrigger();
			GetWorld()->GetTimerManager().ClearTimer(KnockBackDelayTimerHandle);
		}), KnockBackDelayTime, false);

	DeactivateAttackTrigger();
	IsKnockBack = true;
}

void AJamsig::KnockBackEnd()
{
	IsKnockBack = false;
}

void AJamsig::OnJamsigTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActionType == MonsterActionType::DEAD)
		return;
	if (Player == nullptr)
	{
		Player = Cast<APlayerCharacter>(OtherActor);
		Super::PlayerCharacter = Player;
	}
}

void AJamsig::OnJamsigTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AttackAnimationType = MonsterAnimationType::NONE;
}

void AJamsig::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{ 	
	if (StateType == MonsterStateType::CANTACT )
		return;

	TracePlayer = false;
	AttackAnimationType = AttackAnimType;
	if (ActionType != MonsterActionType::ATTACK)
	{
		MonsterController->StopMovement();

		if (MontageMap.Contains(AnimationType))
			JamsigAnimInstance->StopMontage(MontageMap[AnimationType]);

		float RandomValue = FMath::RandRange(0, 100) * 0.01f;
		if (SetActionByRandomMap.Contains(AttackAnimType))
		{
			MonsterMoveEventIndex = 1;
			SetActionByRandomMap[AttackAnimType](RandomValue);
		}
	}
}

void AJamsig::EndAttackTrigger(MonsterAnimationType AttackAnimType)
{
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP)
		return;

	TracePlayer = true;
}

float AJamsig::Die(float Dm)
{
	if (PlayerCharacter->IsLockOn)
		PlayerCharacter->LockOn();

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

	DeactivateAttackTrigger();
	GetWorld()->GetTimerManager().ClearTimer(KnockBackTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(KnockBackDelayTimerHandle);
	
	if (Player->IsLockOn)
	{
		Player->TargetComp = nullptr;
		Player->GetCompsInScreen(Player->TargetCompArray);
		Player->GetFirstTarget();

		if (Player->TargetComp == nullptr)
		{
			Player->LockOn();
		}
		else
		{
			Cast<ABaseCharacter>(Player->TargetComp->GetOwner())->ActivateLockOnImage(true, Player->TargetComp);
		}
	}

	auto index = PlayerCharacter->HitMonsterInfoArray.Find(this);
	//UCombatManager::GetInstance().HitMonsterInfoArray.RemoveAtSwap(index);

	Imotal = true;
	//GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	DeactivateHpBar();
	DeactivateHitCollision();

	JamsigAnimInstance->StopMontage(MontageMap[AnimationType]);

	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();

	ChangeActionType(MonsterActionType::DEAD);
	StateType = MonsterStateType::CANTACT;
	ChangeMontageAnimation(MonsterAnimationType::DEAD);

	GetWorld()->GetTimerManager().SetTimer(MonsterDeadTimer, FTimerDelegate::CreateLambda([=]()
		{
			GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
			auto PoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[44].ObjClass,
			GetActorLocation(), FRotator::ZeroRotator);
			auto CastObj = Cast<AEffectObjectInPool>(PoolObj);
			CastObj->SetEffectType(SelectEffectType::KNIGHT_DEAD);
			CastObj->ActivateCurrentEffect();

			GetMesh()->SetVisibility(false);
			SetActive(false);
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
			SetActorTickEnabled(false);

			//MinusOpacity = true;
		}), 4.5f, false);

	return Dm;
}

void AJamsig::ResumeMontage()
{
	if (MontageMap.Contains(AnimationType))
		JamsigAnimInstance->ResumeMontage(MontageMap[AnimationType]);
}

void AJamsig::Stun()
{
	FDamageEvent DamageEvent;

	IsStun = true;
	JamsigAnimInstance->StopMontage(MontageMap[AnimationType]);
	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	ChangeMontageAnimation(MonsterAnimationType::GROGGY_START);

	TakeDamage(999.f, DamageEvent, PlayerCharacter->GetController(), PlayerCharacter);
}

void AJamsig::ParryingStun()
{
	
}

void AJamsig::MonsterHitStop()
{
	if (MontageMap.Contains(AnimationType))
		JamsigAnimInstance->PauseAnimation(MontageMap[AnimationType]);
}

void AJamsig::ChangeMontageAnimation(MonsterAnimationType type)
{
	if (JamsigAnimInstance == nullptr)
	{
		JamsigAnimInstance = Cast<UJamsigAniminstance>(GetMesh()->GetAnimInstance());
	}
	else
	{
		if (MontageMap.Contains(AnimationType))
			JamsigAnimInstance->StopMontage(MontageMap[AnimationType]);
		AnimationType = type;
		StateType = AnimTypeToStateType[type];

		if (MontageMap.Contains(type))
			JamsigAnimInstance->PlayMontage(MontageMap[type]);
	}
}

float AJamsig::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser))
		return 0.0f;
	
	//KnockBackStart();

	DeactivateHitCollision();

	if (AnimationType == MonsterAnimationType::EXECUTION)
		return 0.f;

	if (MonsterDataStruct.CharacterHp > 0)
	{
		MonsterController->StopMovement();
		JamsigAnimInstance->StopMontage(MontageMap[AnimationType]);
		if (MontageEndEventMap.Contains(AnimationType) && AnimationType != MonsterAnimationType::JAMSIG_STANDUP)
			MontageEndEventMap[AnimationType]();

		if(SitJamsig)
			SitJamsig = false;

		//TODO : 앞 뒤 방향에 따른 피격
		DeactivateRightWeapon();
		ChangeActionType(MonsterActionType::HIT);
		ChangeMontageAnimation(MonsterAnimationType::HIT);
	}
	
	return DamageAmount;
}

void AJamsig::CheckMontageEndNotify()
{
	if (MontageEndEventMap.Contains(AnimationType))
	{
		MontageEndEventMap[AnimationType]();
	}
}

void AJamsig::PlayExecutionAnimation()
{
	//잠식이 처형 애니
	//IsStun = false;
	//CanExecution = false;
	//ChangeMontageAnimation(MonsterAnimationType::EXECUTION);
}


void AJamsig::IsNotifyActive(bool value)
{
	if (NotifyBeginEndEventMap.Contains(AnimationType))
	{
		NotifyBeginEndEventMap[AnimationType][value]();
	}
}

void AJamsig::RespawnCharacter()
{
	if (IsDie)return;
	Super::RespawnCharacter();
	 
	UE_LOG(LogTemp, Warning, TEXT("jamsig respawn"));

	MonsterController->FindPlayer = false;
	JamsigAnimInstance->ResumeMontage(MontageMap[AnimationType]);
	GetWorld()->GetTimerManager().ClearTimer(MonsterDeadTimer);

	ChangeActionType(MonsterActionType::NONE);

	if (SitJamsig)
	{
		GetWorld()->GetTimerManager().SetTimer(SitidleTimerHandle, FTimerDelegate::CreateLambda([=]()
			{
				ChangeMontageAnimation(MonsterAnimationType::JAMSIG_SIT_IDLE);
			}), FMath::RandRange(0.5f, 1.0f), false);
	}
	else
	{
		ChangeMontageAnimation(MonsterAnimationType::IDLE);
	}

	WeaponOpacity = 1.0f;
	MeshOpacity = 1.0f;

	GetCapsuleComponent()->SetCollisionProfileName("AIPhysics");
	SkeletalMeshComp->SetScalarParameterValueOnMaterials("Dither", MeshOpacity);

	MinusOpacity = false;
	OpactiyDeltaTime = 0.f;

	ActivateHitCollision();
	MonsterDataStruct.CharacterHp = MonsterDataStruct.CharacterMaxHp;
	MonsterHPWidget->SetHP(1.0f);
}


