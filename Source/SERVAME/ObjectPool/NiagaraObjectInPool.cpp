#include "../ObjectPool/NiagaraObjectInPool.h"

ANiagaraObjectInPool::ANiagaraObjectInPool()
{
	ParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));
	ParticleSystem->SetupAttachment(RootComponent);
	ParticleSystem->SetCollisionProfileName("Particle");

	MoveComp = CreateDefaultSubobject<UMoveToLocationComp>(TEXT("Movement"));

	PrimaryActorTick.bCanEverTick = false;
}

void ANiagaraObjectInPool::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AObjectPool& objectpool = AObjectPool::GetInstance();
	objectpool.SpawnObject(objectpool.ObjectArray[37].ObjClass, Player->GetActorLocation() + FVector(0, 0, 30.0f), FRotator::ZeroRotator);
	MoveComp->SetComponentTickEnabled(false);
	Player->SetShieldHP(Player->PlayerDataStruct.MaxShieldHP);
	Player->SetSoul(1);
	ReturnObject();
}

void ANiagaraObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);
	ParticleSystem->SetActive(active, false);
	if (active)
	{
		if (LifeTime > 0)
			GetWorldTimerManager().SetTimer(LifeTimer, this, &ANiagaraObjectInPool::ReturnObject, LifeTime);

		MoveComp->SetComponentTickEnabled(IsMove);
		MoveComp->SetTargetLocation(Player);
	}
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