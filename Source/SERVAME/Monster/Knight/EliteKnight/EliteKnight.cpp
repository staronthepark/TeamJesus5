// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteKnight.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	SetActionByRandomMap.Add(MonsterAnimationType::ATTACK1, [&](float percent)
		{
			ChangeActionType(MonsterActionType::ATTACK);
			ChangeMontageAnimation(MonsterAnimationType::SPRINTATTACK);
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
			fDeltaTime = 0;
			SprintTime += DeltaTime;

			//Direction도 보간시켜서 증가시켜야 함.
			BlendSpeed = FMath::Lerp(WalkBlend, RunBlend, SprintTime / SprintDuration);
			KnightAnimInstance->BlendSpeed = BlendSpeed;
			BlendDirection = FMath::Lerp(0.f, 180.f, SprintTime / SprintDuration);
			KnightAnimInstance->BlendDirection = 180.f;

			MonsterDataStruct.RunSpeed = FMath::Lerp(120.f, 240.f, (KnightAnimInstance->BlendSpeed - 300.f) / 300.f);
			GetCharacterMovement()->MaxWalkSpeed = MonsterDataStruct.RunSpeed;

			if (CurrentDistance <= SprintAttackRadius)
			{
				IsMoveStart = false;
				StartAttackTrigger(MonsterAnimationType::SPRINTATTACK);
			}
		}
	}

}
