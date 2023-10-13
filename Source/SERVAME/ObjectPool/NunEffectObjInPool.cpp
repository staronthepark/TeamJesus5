#include "..\NunDamageSphereTriggerComp.h"
#include "NunEffectObjInPool.h"
#include <Kismet/KismetMathLibrary.h>

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

	GetBurstEffectType.Add(EffectType::NONE, EffectType::DARKEFFECTHIT);
	GetBurstEffectType.Add(EffectType::DARKEFFECT, EffectType::DARKEFFECTHIT);
	GetBurstEffectType.Add(EffectType::PRAYEFFECT, EffectType::PRAYHITEFFECT);
	GetBurstEffectType.Add(EffectType::CRYSTALEFFECT, EffectType::CRYSTALEFFECT_BUSRT);
}

void ANunEffectObjInPool::BeginPlay()
{
	Super::BeginPlay();

	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ANunEffectObjInPool::OnProjectileBeginOverlap);
	RangeAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ANunEffectObjInPool::OnRangeAttackBeginOverlap);

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ANunEffectObjInPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsShot)
		SetActorLocation(GetActorLocation() += MoveDir * Speed * DeltaTime);

	if (IsCurve)
	{
		if (IsTrace)
			TargetLoc = PlayerCharacter->GetActorLocation();

		CurvePoint = FMath::VInterpConstantTo(CurvePoint, TargetLoc, GetWorld()->DeltaTimeSeconds, Speed);
		auto Val = FMath::VInterpConstantTo(GetActorLocation(), CurvePoint, GetWorld()->DeltaTimeSeconds, Speed);
		SetActorLocation(Val);

		if (GetActorLocation() == TargetLoc)
		{
			//UE_LOG(LogTemp, Warning, TEXT("GetActorLocation() == TargetLoc"));
			//UE_LOG(LogTemp, Warning, TEXT("%d"), Type);
			IsCurve = false;
			if (GetBurstEffectType.Contains(Type))
				CurrentEffect->SetAsset(GetTypeEffect[GetBurstEffectType[Type]]);
			CurrentEffect->Activate();

			ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
		}
	}
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
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
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

void ANunEffectObjInPool::ShotProjectile(bool val, FVector Target)
{
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			IsCurve = val;
			TargetLoc = Target;

			GetWorld()->GetTimerManager().SetTimer(TraceHandle, FTimerDelegate::CreateLambda([=]()
				{
					IsTrace = false;
				}), TraceTime,false);

			MidPointCalc();
			CurveControlPoint();
		}), Delay, false);
}

void ANunEffectObjInPool::SweepSingle(float delay, float Radius, float damage, bool Isillusion, AController* Controller)
{
	GetWorld()->GetTimerManager().SetTimer(SweepTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, false, this);

			bool bResult = GetWorld()->SweepSingleByChannel(
				OUT HitResult,
				GetActorLocation(),
				GetActorLocation(),
				FQuat::Identity,
				ECollisionChannel::ECC_GameTraceChannel3,
				FCollisionShape::MakeSphere(Radius),
				Params);

			FColor DrawColor;

			if (bResult)
				DrawColor = FColor::Green;
			else
				DrawColor = FColor::Red;

			//DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 16, DrawColor, false, 2.f);

			//CameraShake(PlayerCameraShake);
			//VibrateGamePad(1.0f, 0.5f);

			if (bResult && HitResult.GetActor())
			{
				FDamageEvent DamageEvent;
				auto Player = Cast<APlayerCharacter>(HitResult.GetActor());

				if (Isillusion || Player == nullptr)
					return;

				Player->TakeDamage(damage, DamageEvent, Controller, this);
			}
		}), delay, false);
}

void ANunEffectObjInPool::MidPointCalc()
{
	auto Loc = TargetLoc - GetActorLocation();
	auto RandVal = FMath::RandRange(MinCurvePointDistance, MaxCurvePointDistance);
	MidPoint = (Loc * RandVal) + GetActorLocation();
}

void ANunEffectObjInPool::CurveControlPoint()
{
	auto Val = UKismetMathLibrary::FindLookAtRotation(MidPoint,TargetLoc);
	FVector X_Vec, Y_Vec, Z_Vec;
	UKismetMathLibrary::BreakRotIntoAxes(Val, X_Vec, Y_Vec, Z_Vec);

	auto AddVal = UKismetMathLibrary::Add_VectorFloat(Y_Vec, FMath::RandRange(MinCurveRadius, MaxCurveRadius));
	auto RotateAngleVal = AddVal.RotateAngleAxis(FMath::RandRange(MinAngle, MaxAngle), X_Vec);

	CurvePoint = MidPoint + RotateAngleVal;
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
	IsCurve = false;
	SetActorTickEnabled(false);
	DeactivateCurrentEffect();

	if (Type == EffectType::NONE)
		return;

	if (GetBurstEffectType.Contains(Type))
	{
		Type = GetBurstEffectType[Type];
		CurrentEffect->SetAsset(GetTypeEffect[Type]);
		CurrentEffect->Activate();
	}

	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this))
	{

	}
}

void ANunEffectObjInPool::OnRangeAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
