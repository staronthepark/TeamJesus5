// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteKnight.h"

AEliteKnight::AEliteKnight()
{
	SetActionByRandomMap.Add(MonsterAnimationType::ATTACK1, [&](float percent)
		{
			if (percent >= 0.5)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::ATTACK1);
			}
			else if (percent < 0.5f)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::POWERATTACK1);
			}
		});

}

void AEliteKnight::BeginPlay()
{
	Super::BeginPlay();

	MyMonsterType = MonsterType::ELITEKNIGHT;
}

void AEliteKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
