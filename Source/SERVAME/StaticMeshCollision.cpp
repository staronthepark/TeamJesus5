#include "StaticMeshCollision.h"
#include ".\ObjectPool\ObjectPool.h"

AStaticMeshCollision::AStaticMeshCollision()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Cube");
}

void AStaticMeshCollision::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AStaticMeshCollision::OnOverlapBegin);
	Mesh->OnComponentEndOverlap.AddDynamic(this, &AStaticMeshCollision::OnOverlapEnd);
}

void AStaticMeshCollision::SpawnEffect()
{
	//if (MaterialType == EMaterialType::NONE)return;
	//int Index = 12;
	//if (MaterialType == EMaterialType::BLOOD)
	//{
	//	Index = 33;
	//}
	//for (int32 i = 0; i < CompArray.Num(); i++)
	//{
	//	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[Index].ObjClass, CompArray[i]->GetComponentLocation(), FRotator(90, 0, 0));
	//}
	//GetWorldTimerManager().SetTimer(TimeHandle, this, &AStaticMeshCollision::SpawnEffect, 0.02f);
}

void AStaticMeshCollision::OverlapBegin(UPrimitiveComponent* OtherComp)
{
	//CompArray.Add(OtherComp);
	//SpawnEffect();
}

void AStaticMeshCollision::OverlapEnd(UPrimitiveComponent* OtherComp)
{
	//CompArray.Remove(OtherComp);
	//if (CompArray.Num() <= 0)
	//{
	//	GetWorldTimerManager().ClearTimer(TimeHandle);
	//}
}

void AStaticMeshCollision::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapBegin(OtherComp);
}

void AStaticMeshCollision::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlapEnd(OtherComp);
}