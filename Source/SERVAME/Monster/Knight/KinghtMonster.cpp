
#include "KinghtMonster.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RandomStream.h"
#include "KnightAttackTriggerComp.h"
#include "..\..\Manager\CombatManager.h"
#include "KnightAttackTriggerComp.h"
#include "GameFramework/CharacterMovementComponent.h"

AKinghtMonster::AKinghtMonster()
{
	AttackTrigger = CreateDefaultSubobject<UKnightAttackTriggerComp>(TEXT("AttackTriggerCollision"));
	AttackTrigger->SetupAttachment(GetMesh());

	DashAttackTrigger = CreateDefaultSubobject<UKnightAttackTriggerComp>(TEXT("DashAttackTriggerCollision"));
	DashAttackTrigger->SetupAttachment(GetMesh());

	SetActionByRandomMap.Add(MonsterAnimationType::ATTACK1, [&](float percent)
		{
			if (percent >= 0.5)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::ATTACK1);
			}
			else if (percent < 0.5f)
			{
				//UGameplayStatics::SetGlobalTimeDilation(monster, 0.1f);
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::POWERATTACK1);
			}
		});
}

void AKinghtMonster::BeginPlay()
{
	Super::BeginPlay(); 

	SetActive(true);

	PlayerCharacter = nullptr;

	KnightAnimInstance = Cast<UKnightAnimInstance>(GetMesh()->GetAnimInstance());

	TargetDetectionCollison->OnComponentBeginOverlap.AddDynamic(this, &AKinghtMonster::OnKnightTargetDetectionBeginOverlap);
	TargetDetectionCollison->OnComponentEndOverlap.AddDynamic(this, &AKinghtMonster::OnKnightTargetDetectionEndOverlap);

	if (KnightAnimInstance != nullptr)
	{
		KnightAnimInstance->InterpStart.AddUObject(this, &AKinghtMonster::InterpStart);
		KnightAnimInstance->InterpEnd.AddUObject(this, &AKinghtMonster::InterpEnd);
		KnightAnimInstance->KnockBackStart.AddUObject(this, &AKinghtMonster::KnockBackStart);
		KnightAnimInstance->KnockBackEnd.AddUObject(this, &AKinghtMonster::KnockBackEmd);
	}
}

void AKinghtMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInterpStart)
		InterpMove();
}

void AKinghtMonster::RespawnCharacter()
{
	Super::RespawnCharacter();

	WeaponOpacity = 0.171653f;
	MeshOpacity = 0.171653f;

	ActivateHitCollision();
	MonsterDataStruct.CharacterHp = MonsterDataStruct.CharacterMaxHp;
	MonsterHPWidget->SetHP(1.0f);
}

void AKinghtMonster::ResumeMontage()
{
	if (MontageMap.Contains(AnimationType))
		KnightAnimInstance->ResumeMontage(MontageMap[AnimationType]);
}

void AKinghtMonster::InterpStart() { IsInterpStart = true; }

void AKinghtMonster::InterpEnd() { IsInterpStart = false; }

void AKinghtMonster::KnockBackStart()
{
	GetWorld()->GetTimerManager().SetTimer(KnockBackTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			IsKnockBack = false;
			GetWorld()->GetTimerManager().ClearTimer(KnockBackTimerHandle);
		}), KnockBackTime, false);

	GetWorld()->GetTimerManager().SetTimer(KnockBackDelayTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			ActivateAttackTrigger();
			MontageEndEventMap[MonsterAnimationType::HIT]();
			GetWorld()->GetTimerManager().ClearTimer(KnockBackDelayTimerHandle);
		}), KnockBackDelayTime, false);

	StopAnimMontage(MontageMap[AnimationType]); 
	DeactivateAttackTrigger();
	IsKnockBack = true; 
}

void AKinghtMonster::Stun()
{
	KnightAnimInstance->StopMontage(MontageMap[AnimationType]);
	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	ChangeMontageAnimation(MonsterAnimationType::DEAD);
}

void AKinghtMonster::KnockBackEmd() 
{ 
	IsKnockBack = false; 
}

void AKinghtMonster::ChangeMontageAnimation(MonsterAnimationType type)
{
	if (KnightAnimInstance == nullptr)
	{
		KnightAnimInstance = Cast<UKnightAnimInstance>(GetMesh()->GetAnimInstance());
	}
	else
	{
		KnightAnimInstance->StopMontage(MontageMap[AnimationType]);
		AnimationType = type;
		StateType = AnimTypeToStateType[type];
		KnightAnimInstance->PlayMontage(MontageMap[type]);
	}
}

void AKinghtMonster::InterpMove()
{
	auto NewLoc = FMath::Lerp(GetActorLocation(), PlayerCharacter->GetActorLocation(), 0.1f);
	SetActorLocation(NewLoc);
}

void AKinghtMonster::HitStop()
{
	Super::HitStop();
	if (MontageMap.Contains(AnimationType))
		KnightAnimInstance->PauseAnimation(MontageMap[AnimationType]);
}
void AKinghtMonster::ActivateAttackTrigger()
{
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DashAttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AKinghtMonster::DeactivateAttackTrigger()
{
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DashAttackTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKinghtMonster::OnKnightTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO : 
	//소환되자 마자 플레이어 추격 하도록 변경?

	if (ActionType == MonsterActionType::DEAD)
		return;
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
	}
}

void AKinghtMonster::OnKnightTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AttackAnimationType = MonsterAnimationType::NONE;
}

void AKinghtMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	TracePlayer = false;
	if (StateType == MonsterStateType::CANTACT || GetMesh()->GetCollisionProfileName() == "Ragdoll" || IsKnockBack == true)
		return;
	AttackAnimationType = AttackAnimType;
	if (ActionType != MonsterActionType::ATTACK)
	{
		MonsterController->StopMovement();
		KnightAnimInstance->StopMontage(MontageMap[AnimationType]);

		float RandomValue = FMath::RandRange(0, 100) * 0.01f;
		if (SetActionByRandomMap.Contains(AttackAnimType))
		{
			MonsterMoveEventIndex = 1;
			SetActionByRandomMap[AttackAnimType](RandomValue);
		}
	}
}

void AKinghtMonster::EndAttackTrigger(MonsterAnimationType AttackAnimType)
{
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP)
		return;
	TracePlayer = true;
}

void AKinghtMonster::Rotate()
{
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP
		|| AnimationType == MonsterAnimationType::EXECUTION)
		return;
	SetActorRotation(FMath::Lerp(GetActorRotation(), YawRotation, MonsterDataStruct.RotateSpeed * fDeltaTime));
}

float AKinghtMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (Imotal)
	{
		return 0;
	}

	GetWorldTimerManager().SetTimer(HpTimer, this, &AKinghtMonster::DeactivateHpBar, 3.0f);

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
		ChangeMontageAnimation(MonsterAnimationType::DEAD);
		return DamageAmount;
	}

	if (DamageAmount >= 30)
	{
		MonsterController->StopMovement();
		KnightAnimInstance->StopMontage(MontageMap[AnimationType]);
		if (MontageEndEventMap.Contains(AnimationType))
			MontageEndEventMap[AnimationType]();
		
		ChangeActionType(MonsterActionType::HIT);
		ChangeMontageAnimation(MonsterAnimationType::HIT);
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
	ChangeMontageAnimation(MonsterAnimationType::EXECUTION);
}
