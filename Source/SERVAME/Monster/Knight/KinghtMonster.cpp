// Fill out your copyright notice in the Description page of Project Settings.

#include "KinghtMonster.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RandomStream.h"
#include "..\..\Manager\CombatManager.h"
#include "GameFramework/CharacterMovementComponent.h"

AKinghtMonster::AKinghtMonster()
{
	IsDetect = false;

	//TargetDetectionCollison = CreateDefaultSubobject<USphereComponent>(TEXT("Target Detection Collider"));
	//TargetDetectionCollison->SetupAttachment(RootComponent);
	//TargetDetectionCollison->SetCollisionProfileName("Detection");
	//TargetDetectionCollison->SetSphereRadius(500.0f);

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

	AnimTypeToStateType.Add(KnightAnimationType::FORWARDMOVE, KnightStateType::NONE);
	AnimTypeToStateType.Add(KnightAnimationType::LEFTMOVE, KnightStateType::NONE);
	AnimTypeToStateType.Add(KnightAnimationType::RIGHTMOVE, KnightStateType::NONE);
	AnimTypeToStateType.Add(KnightAnimationType::BACKMOVE, KnightStateType::NONE);

	AnimTypeToStateType.Add(KnightAnimationType::IDLE, KnightStateType::NONE);

	AnimTypeToStateType.Add(KnightAnimationType::ATTACK1, KnightStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(KnightAnimationType::POWERATTACK1, KnightStateType::BEFOREATTACK);

	AnimTypeToStateType.Add(KnightAnimationType::HIT, KnightStateType::CANTACT);
	AnimTypeToStateType.Add(KnightAnimationType::DEAD, KnightStateType::CANTACT);
	AnimTypeToStateType.Add(KnightAnimationType::DEADLOOP, KnightStateType::CANTACT);
	AnimTypeToStateType.Add(KnightAnimationType::EXECUTION, KnightStateType::CANTACT);
}

void AKinghtMonster::BeginPlay()
{
	Super::BeginPlay();

}

void AKinghtMonster::Tick(float DeltaTime)
{
}

void AKinghtMonster::ActivateLockOnImage(bool value)
{
}

void AKinghtMonster::BeforeAttackNotify(bool value)
{
}

void AKinghtMonster::AfterAttackNotify(bool value)
{
}

void AKinghtMonster::IsNotifyActive(bool value)
{
}

void AKinghtMonster::RespawnCharacter()
{
}

void AKinghtMonster::ResumeMontage()
{
}

void AKinghtMonster::HitStop()
{
}

void AKinghtMonster::ChangeMontageAnimation(KnightAnimationType type)
{
}

void AKinghtMonster::ChangeActionType(KnightActionType type)
{
}

void AKinghtMonster::DeactivateHpBar()
{
}

void AKinghtMonster::OnTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AKinghtMonster::OnTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AKinghtMonster::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AKinghtMonster::OnSMOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AKinghtMonster::OnSMOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AKinghtMonster::OnParryingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AKinghtMonster::OnGrabCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AKinghtMonster::StartAttackTrigger(KnightAnimationType AttackAnimType)
{
}

void AKinghtMonster::EndAttackTrigger(KnightAnimationType AttackAnimType)
{
}

void AKinghtMonster::ShotProjectile()
{
}

void AKinghtMonster::Rotate()
{
}

void AKinghtMonster::CatchByPlayer()
{
}

void AKinghtMonster::LaunchCharacter(FVector Dir, float Power)
{
}

float AKinghtMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void AKinghtMonster::CheckMontageEndNotify()
{
}

void AKinghtMonster::PlayExecutionAnimation()
{
}
