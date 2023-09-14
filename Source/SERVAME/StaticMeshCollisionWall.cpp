// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticMeshCollisionWall.h"
#include ".\ObjectPool\ObjectPool.h"

void AStaticMeshCollisionWall::BeginPlay()
{
	Super::BeginPlay();
}

void AStaticMeshCollisionWall::OverlapBegin(UPrimitiveComponent* OtherComp)
{
	//int Index = 2;
	//if (MaterialType == EMaterialType::BLOOD)
	//{
	//	Index = 33;
	//}
	//else
	//{
	//	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[Index].ObjClass, OtherComp->GetComponentLocation(), FRotator(90, 0, 0));
	//}
}