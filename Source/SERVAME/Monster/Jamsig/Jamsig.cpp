#include "Jamsig.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RandomStream.h"
#include "JamsigAttackTriggerComp.h"
#include "..\..\Manager\CombatManager.h"
#include "..\..\ObjectPool\EffectObjectInPool.h"
#include "GameFramework/CharacterMovementComponent.h"

AJamsig::AJamsig()
{
	AttackTrigger = CreateDefaultSubobject<UJamsigAttackTriggerComp>(TEXT("AttackTriggerCollision"));
	AttackTrigger->SetupAttachment(GetMesh());

	ParryingCollision1->SetupAttachment(GetMesh(), FName("Bip001-R-Hand"));
	SwordTrailComp->SetupAttachment(GetMesh(), FName("Bip001-R-Hand"));
	WeaponCollision->SetupAttachment(GetMesh(), FName("Bip001-R-Hand"));
	WeaponOverlapStaticMeshCollision->SetupAttachment(GetMesh(), FName("Bip001-R-Hand"));
	WeaponOverlapStaticMeshCollision->SetCollisionProfileName("Weapon");
}

void AJamsig::BeginPlay()
{
	Super::BeginPlay();

	DeactivateHpBar();

	SetActive(true);
	PlayerCharacter = nullptr;

	JamsigAnimInstance = Cast<UJamsigAniminstance>(GetMesh()->GetAnimInstance());

	TargetDetectionCollison->OnComponentBeginOverlap.AddDynamic(this, &AJamsig::OnJamsigTargetDetectionBeginOverlap);
	TargetDetectionCollison->OnComponentEndOverlap.AddDynamic(this, &AJamsig::OnJamsigTargetDetectionEndOverlap);
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

void AJamsig::OnJamsigTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActionType == MonsterActionType::DEAD)
		return;
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		Super::PlayerCharacter = PlayerCharacter;
	}
}

void AJamsig::OnJamsigTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AttackAnimationType = MonsterAnimationType::NONE;
}

void AJamsig::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	TracePlayer = false;
	MonsterController->StopMovement();
	ChangeMontageAnimation(MonsterAnimationType::IDLE);

	//TracePlayer = false;
	//if (StateType == MonsterStateType::CANTACT )
	//	return;
	//AttackAnimationType = AttackAnimType;
	//if (ActionType != MonsterActionType::ATTACK)
	//{
	//	MonsterController->StopMovement();

	//	if (MontageMap.Contains(AnimationType))
	//		JamsigAnimInstance->StopMontage(MontageMap[AnimationType]);

	//	float RandomValue = FMath::RandRange(0, 100) * 0.01f;
	//	if (SetActionByRandomMap.Contains(AttackAnimType))
	//	{
	//		MonsterMoveEventIndex = 1;
	//		SetActionByRandomMap[AttackAnimType](RandomValue);
	//	}
	//}
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
	{
		PlayerCharacter->TargetComp = nullptr;
		PlayerCharacter->GetCompsInScreen(PlayerCharacter->TargetCompArray);
		PlayerCharacter->GetFirstTarget();

		if (PlayerCharacter->TargetComp == nullptr)
		{
			PlayerCharacter->LockOn();
		}
		else
		{
			Cast<ABaseCharacter>(PlayerCharacter->TargetComp->GetOwner())->ActivateLockOnImage(true, PlayerCharacter->TargetComp);
		}
	}

	auto index = UCombatManager::GetInstance().HitMonsterInfoArray.Find(this);
	UCombatManager::GetInstance().HitMonsterInfoArray.RemoveAt(index);

	Imotal = true;
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
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
			auto PoolObj = AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[44].ObjClass,
			GetActorLocation(), FRotator::ZeroRotator);
			auto CastObj = Cast<AEffectObjectInPool>(PoolObj);
			CastObj->SetEffectType(SelectEffectType::KNIGHT_DEAD);
			CastObj->ActivateCurrentEffect();

			GetMesh()->SetVisibility(false);
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
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DeactivateHitCollision();

	if (AnimationType == MonsterAnimationType::EXECUTION)
		return 0.f;

	//TODO : 잠식이 피격 애니 나오면 코드 수정
	//if (DamageAmount >= 30 && MonsterDataStruct.CharacterHp > 0)
	//{
	//	if (CanCancle)
	//	{
	//		MonsterController->StopMovement();

	//		JamsigAnimInstance->StopMontage(MontageMap[AnimationType]);
	//		if (MontageEndEventMap.Contains(AnimationType))
	//			MontageEndEventMap[AnimationType]();

	//		//TODO : 앞 뒤 방향에 따른 피격
	//		ChangeActionType(MonsterActionType::HIT);
	//		ChangeMontageAnimation(HitType);
	//	}
	//}

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
	Super::RespawnCharacter();

	JamsigAnimInstance->ResumeMontage(MontageMap[AnimationType]);
	GetWorld()->GetTimerManager().ClearTimer(MonsterDeadTimer);

	ChangeActionType(MonsterActionType::NONE);
	ChangeMontageAnimation(MonsterAnimationType::IDLE);

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


