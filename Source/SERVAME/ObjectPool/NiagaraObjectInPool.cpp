#include "../ObjectPool/NiagaraObjectInPool.h"

ANiagaraObjectInPool::ANiagaraObjectInPool()
{
	ParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));
	ParticleSystem->SetupAttachment(RootComponent);
	ParticleSystem->SetCollisionProfileName("Particle");

	MoveComp = CreateDefaultSubobject<UMoveToLocationComp>(TEXT("Movement"));

	PrimaryActorTick.bCanEverTick = false;
}

void ANiagaraObjectInPool::StartMove()
{
	MoveComp->SetTargetLocation(Player);
	MoveComp->SetComponentTickEnabled(true);
	ParticleSystem->SetActive(true, false);
}

void ANiagaraObjectInPool::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AObjectPool& objectpool = AObjectPool::GetInstance();
	objectpool.SpawnObject(objectpool.ObjectArray[37].ObjClass, Player->GetActorLocation() + FVector(0, 0, 30.0f), FRotator::ZeroRotator);
	MoveComp->SetComponentTickEnabled(false);
	Player->SetSoul(1);
	ReturnObject();
}

void ANiagaraObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);
	if (active)
	{
		if (LifeTime > 0)
			GetWorldTimerManager().SetTimer(LifeTimer, this, &ANiagaraObjectInPool::ReturnObject, LifeTime);

		if (IsMove)
		{
			GetWorldTimerManager().SetTimer(MoveStartTimer, this, &ANiagaraObjectInPool::StartMove, 0.3f);
		}
		else
		{
			ParticleSystem->SetActive(true, false);
		}
		return;
	}
	ParticleSystem->SetActive(active, false);
}

void ANiagaraObjectInPool::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*>ActorCompArray;
	ActorCompArray = GetComponentsByTag(UBoxComponent::StaticClass(), FName("Overlap"));
	if (ActorCompArray.Num() > 0)
	{
		OverlapComp = Cast<UBoxComponent>(ActorCompArray[0]);
		OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ANiagaraObjectInPool::OverlapBegin);
	}

	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}