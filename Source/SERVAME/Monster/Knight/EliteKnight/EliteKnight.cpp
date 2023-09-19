// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteKnight.h"

AEliteKnight::AEliteKnight()
{
	SetActionByRandomMap.Add(MonsterAnimationType::ATTACK1, [&](float percent)
		{
			if (percent <= 0.2f)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::ATTACK1);
			}
			else if (percent> 0.2f && percent < 0.7f)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::POWERATTACK1);
			}
			else
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::STINGaTTACK1);
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
