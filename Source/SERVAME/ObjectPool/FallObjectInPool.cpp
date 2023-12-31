// Fill out your copyright notice in the Description page of Project Settings.


#include "FallObjectInPool.h"

AFallObjectInPool::AFallObjectInPool()
{
	PrimaryActorTick.bCanEverTick = true;

	HitCollision = CreateDefaultSubobject<UBoxComponent>("HitCollision");
	HitCollision->SetupAttachment(RootComponent);
	HitCollision->SetCollisionProfileName("AIProjectile");

	GroundHitCollision = CreateDefaultSubobject<UBoxComponent>("GroundHitCollision");
	GroundHitCollision->SetupAttachment(HitCollision);

	CrossEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Cross Effect"));
	CrossEffect->SetupAttachment(HitCollision);

	CrossBurstEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Cross Burst Effect"));
	CrossBurstEffect->SetupAttachment(HitCollision);

	DustEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Dust Effect"));
	DustEffect->SetupAttachment(HitCollision);
}

void AFallObjectInPool::BeginPlay()
{
	Super::BeginPlay();

	CrossEffect->Deactivate();
	SetActorTickEnabled(false);
	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &AFallObjectInPool::OnCollisionBeginOverlap);
	GroundHitCollision->OnComponentBeginOverlap.AddDynamic(this, &AFallObjectInPool::OnGroundOverlap);
	GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	Boss2 = Cast<AJesusBoss2>(UGameplayStatics::GetActorOfClass(GetWorld(), AJesusBoss2::StaticClass()));
}

void AFallObjectInPool::Tick(float DeltaTime)
{
	if (!IsHitGround)
		SetActorRelativeLocation(GetActorLocation() += MoveDir * DeltaTime * FallSpeed);
}

void AFallObjectInPool::ReturnObject()
{
	Super::ReturnObject();
	SetActorTickEnabled(false);
}

void AFallObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);

	SetActorTickEnabled(false);

	MoveDir = -GetActorUpVector();


	if (LifeTime > 0 && active)
	{
		CrossEffect->Activate();

		CrossBurstEffect->Deactivate();
		DustEffect->Deactivate();

		GetWorldTimerManager().SetTimer(LifeTimer, this, &AFallObjectInPool::ReturnObject, LifeTime);
	}
}

void AFallObjectInPool::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);

	if (OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this))
	{
		HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Boss2->PlayMonsterSoundInPool(EMonsterAudioType::BOSS2_CROSS_EXPLOSION);
	}
}

void AFallObjectInPool::OnGroundOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�΋H�� ������Ʈ �α� ����ֱ�.
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherComp->GetName());

	IsHitGround = true;
	SetActorTickEnabled(false);

	CrossBurstEffect->Activate();
	DustEffect->Activate();

	HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GroundHitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Boss2->PlayMonsterSoundInPool(EMonsterAudioType::BOSS2_CROSS_EXPLOSION);
}
