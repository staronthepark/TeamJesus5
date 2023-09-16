#include "..\NunDamageSphereTriggerComp.h"
#include "NunEffectObjInPool.h"

ANunEffectObjInPool::ANunEffectObjInPool()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	RootComponent = RootComp;

	CurrentEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CurrentEffect"));
	CurrentEffect->SetupAttachment(RootComponent);

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	ProjectileCollision->SetupAttachment(CurrentEffect);

	RangeAttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RangeAttackCollision"));
	RangeAttackCollision->SetupAttachment(RootComponent);

	DamageSphereTriggerComp = CreateDefaultSubobject<UNunDamageSphereTriggerComp>(TEXT("DamageSphere_a"));
	DamageSphereTriggerComp->SetupAttachment(RootComponent);

	GetBurstEffectType.Add(EffectType::DARKEFFECT, EffectType::DARKEFFECTHIT);
	GetBurstEffectType.Add(EffectType::PRAYEFFECT, EffectType::PRAYHITEFFECT);
	GetBurstEffectType.Add(EffectType::CRYSTALEFFECT, EffectType::CRYSTALEFFECT_BUSRT);
}

void ANunEffectObjInPool::BeginPlay()
{
	Super::BeginPlay();

	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ANunEffectObjInPool::OnProjectileBeginOverlap);
	RangeAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ANunEffectObjInPool::OnRangeAttackBeginOverlap);
}

void ANunEffectObjInPool::Tick(float DeltaTime)
{
	if (IsShot)
		SetActorLocation(GetActorLocation() += MoveDir * Speed * DeltaTime);
}

void ANunEffectObjInPool::SetActive(bool active)
{
	Super::SetActive(active);

	DamageSphereTriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RangeAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MoveDir = FVector::ZeroVector;

	if (LifeTime > 0 && active)
		GetWorldTimerManager().SetTimer(LifeTimer, this, &ANunEffectObjInPool::ReturnObject, LifeTime);
}

void ANunEffectObjInPool::ReturnObject()
{
	Super::ReturnObject();
	SetActorTickEnabled(false);
}

void ANunEffectObjInPool::ShotProjectile(ABaseCharacter* Player)
{
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, FTimerDelegate::CreateLambda([=]()
	{		
		MoveDir = Player->GetActorLocation() - GetActorLocation();
		MoveDir.Normalize();
		IsShot = true;
	}), Delay, false);
}

void ANunEffectObjInPool::ShotProjectile(FVector Target)
{
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			MoveDir = Target - GetActorLocation();
			MoveDir.Normalize();
			IsShot = true;
		}), Delay, false);
}

void ANunEffectObjInPool::SetCurrentEffect(EffectType type)
{
	Type = type;
	CurrentEffect->SetAsset(GetTypeEffect[type]);
}

void ANunEffectObjInPool::ActivateCurrentEffect()
{
	SetActorTickEnabled(true);
	CurrentEffect->Activate();
}

void ANunEffectObjInPool::DeactivateCurrentEffect()
{
	SetActorTickEnabled(false);
	CurrentEffect->DeactivateImmediate();
}

void ANunEffectObjInPool::DeactivateDamageSphere(float time)
{
	GetWorld()->GetTimerManager().SetTimer(DotTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			DamageSphereTriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}), time, false);
}

void ANunEffectObjInPool::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	DeactivateCurrentEffect();

	Type = GetBurstEffectType[Type];

	CurrentEffect->SetAsset(GetTypeEffect[Type]);
	CurrentEffect->Activate();

	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this))
	{

	}
}

void ANunEffectObjInPool::OnRangeAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
