// Fill out your copyright notice in the Description page of Project Settings.


#include "NunMonster.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RandomStream.h"
#include "..\..\Manager\CombatManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NunAttackTriggerComp.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"

ANunMonster::ANunMonster()
{
	AttackTrigger = CreateDefaultSubobject<UNunAttackTriggerComp>(TEXT("AttackTriggerCollision"));
	AttackTrigger->SetupAttachment(GetMesh());

}

void ANunMonster::BeginPlay()
{
	Super::BeginPlay();

	NunAnimInstance = Cast<UNumAnimInstance>(GetMesh()->GetAnimInstance());
	WeaponOverlapStaticMeshCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponCollision->SetupAttachment(GetMesh(), FName("Nun_Weapon_Bone"));
}

void ANunMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANunMonster::OnNunTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ActivateHpBar();

	if (ActionType == MonsterActionType::DEAD)
		return;
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	}
}

void ANunMonster::OnNunTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AttackAnimationType = MonsterAnimationType::NONE;
}

void ANunMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	TracePlayer = false;
	if (StateType == MonsterStateType::CANTACT)
		return;
	AttackAnimationType = AttackAnimType;
	if (ActionType != MonsterActionType::ATTACK)
	{
		MonsterController->StopMovement();
		NunAnimInstance->StopMontage(MontageMap[AnimationType]);

		float RandomValue = FMath::RandRange(0, 100) * 0.01f;
		if (SetActionByRandomMap.Contains(AttackAnimType))
		{
			MonsterMoveEventIndex = 1;
			SetActionByRandomMap[AttackAnimType](RandomValue);
		}
	}
}

void ANunMonster::EndAttackTrigger(MonsterAnimationType AttackAnimType)
{
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP)
		return;
	TracePlayer = true;
}

float ANunMonster::Die(float Dm)
{
	return 0.0f;
}

void ANunMonster::Stun()
{
	NunAnimInstance->StopMontage(MontageMap[AnimationType]);
	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	ChangeMontageAnimation(MonsterAnimationType::DEAD);
}

void ANunMonster::MonsterHitStop()
{
	if (MontageMap.Contains(AnimationType))
		NunAnimInstance->PauseAnimation(MontageMap[AnimationType]);
}

void ANunMonster::ChangeMontageAnimation(MonsterAnimationType type)
{
	if (NunAnimInstance == nullptr)
	{
		NunAnimInstance = Cast<UNumAnimInstance>(GetMesh()->GetAnimInstance());
	}
	else
	{
		NunAnimInstance->StopMontage(MontageMap[AnimationType]);
		AnimationType = type;
		StateType = AnimTypeToStateType[type];
		NunAnimInstance->PlayMontage(MontageMap[type]);
	}
}

float ANunMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void ANunMonster::CheckMontageEndNotify()
{
}

void ANunMonster::PlayExecutionAnimation()
{
}

void ANunMonster::IsNotifyActive(bool value)
{
}

void ANunMonster::RespawnCharacter()
{
	Super::RespawnCharacter();

	WeaponOpacity = 0.171653f;
	MeshOpacity = 0.171653f;

	ActivateHitCollision();
	MonsterDataStruct.CharacterHp = MonsterDataStruct.CharacterMaxHp;
	MonsterHPWidget->SetHP(1.0f);
}

void ANunMonster::ResumeMontage()
{
	if (MontageMap.Contains(AnimationType))
		NunAnimInstance->ResumeMontage(MontageMap[AnimationType]);
}
