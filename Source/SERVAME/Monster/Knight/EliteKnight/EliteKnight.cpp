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

	if (IsMoveStart)
	{
		fDeltaTime += DeltaTime;
		if (fDeltaTime >= MinWalkTime)
		{
			//블렌드 스페이스 적용
			SprintTime += DeltaTime;
			KnightAnimInstance->BlendDirection = 180.f;
			BlendSpeed = FMath::Lerp(WalkBlend, RunBlend, SprintTime / SprintDuration);
			KnightAnimInstance->BlendSpeed = BlendSpeed;
		}

		if (CurrentDistance <= SprintAttackRadius)
		{
			ChangeActionType(MonsterActionType::ATTACK);
			ChangeMontageAnimation(MonsterAnimationType::SPRINTATTACK);
		}
	}

}

void AEliteKnight::RunBlending()
{
	IsMoveStart = false;

	KnightAnimInstance->BlendDirection = 180.f;
}

