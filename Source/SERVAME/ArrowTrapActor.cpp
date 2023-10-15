// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowTrapActor.h"
#include "./ObjectPool/ObjectPool.h"

AArrowTrapActor::AArrowTrapActor()
{
	SceneComp1 = CreateDefaultSubobject<USceneComponent>("SceneComp1");
	SceneComp2 = CreateDefaultSubobject<USceneComponent>("SceneComp2");
	SceneComp3 = CreateDefaultSubobject<USceneComponent>("SceneComp3");
	SceneComp1->SetupAttachment(SceneComp);
	SceneComp2->SetupAttachment(SceneComp);
	SceneComp3->SetupAttachment(SceneComp);
}

void AArrowTrapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArrowTrapActor::BeginPlay()
{
	Super::BeginPlay();

}

void AArrowTrapActor::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();
}

void AArrowTrapActor::EndTriggerEvent()
{
	Super::EndTriggerEvent();
}

void AArrowTrapActor::EnableEvent()
{
	Super::EnableEvent();

		EnableTrap();
}

void AArrowTrapActor::EnableTrap()
{
	AObjectPool& ObjectPool = AObjectPool::GetInstance();

	ObjectPool.SpawnObject(ObjectPool.ObjectArray[45].ObjClass, SceneComp1->GetComponentLocation(), GetActorRotation());
	ObjectPool.SpawnObject(ObjectPool.ObjectArray[45].ObjClass, SceneComp2->GetComponentLocation(), GetActorRotation());
	ObjectPool.SpawnObject(ObjectPool.ObjectArray[45].ObjClass, SceneComp3->GetComponentLocation(), GetActorRotation());
}