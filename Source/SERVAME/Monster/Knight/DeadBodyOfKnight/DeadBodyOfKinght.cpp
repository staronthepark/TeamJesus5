// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadBodyOfKinght.h"

ADeadBodyOfKinght::ADeadBodyOfKinght()
{
}

void ADeadBodyOfKinght::BeginPlay()
{
	Super::BeginPlay();

	MyMonsterType = MonsterType::DEADBODYOFKNIGHT;
}

void ADeadBodyOfKinght::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
