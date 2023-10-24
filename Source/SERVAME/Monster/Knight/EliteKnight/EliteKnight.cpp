// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteKnight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "..\KnightAttackTriggerComp.h"

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

	SetActionByRandomMap.Add(MonsterAnimationType::SPRINTATTACK, [&](float percent)
		{
			ChangeActionType(MonsterActionType::ATTACK);
			ChangeMontageAnimation(MonsterAnimationType::SPRINTATTACK);
		});
}

void AEliteKnight::BeginPlay()
{
	Super::BeginPlay();
	DeactivateHpBar();
	MyMonsterType = MonsterType::ELITEKNIGHT;
}

void AEliteKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsMoveStart)
	{
		SprintDeltaTime += DeltaTime;

		//UE_LOG(LogTemp, Warning, TEXT("%f"), SprintDeltaTime);
		//UE_LOG(LogTemp, Warning, TEXT("%f"), MinWalkTime);
		//UE_LOG(LogTemp, Warning, TEXT("Blend Speed : %f"), KnightAnimInstance->BlendSpeed);

		if (SprintDeltaTime >= MinWalkTime)
		{
			ChangeActionType(MonsterActionType::SPRINT);

			SprintTime += DeltaTime;
			BlendDirection = FMath::Lerp(0.f, 180.f, SprintTime / SprintDuration);
			FMath::Clamp(BlendDirection, 0, 180);
			KnightAnimInstance->BlendDirection = BlendDirection;

			if (CurrentDistance <= SprintAttackRadius)
			{
				IsMoveStart = false;
				StartAttackTrigger(MonsterAnimationType::SPRINTATTACK);
			}
		}
	}
	else
	{
		SprintTime = 0;
		SprintDeltaTime = 0;
		KnightAnimInstance->BlendDirection = 0.f;
	}
}

float AEliteKnight::Die(float Dm)
{
	Super::Die(Dm);

	if (IsBoss)
	{
		MonsterController->BossUI->PlayBossDiedAnimtion();
		MonsterController->BossUI->RemoveFromParent();
	}
	return 0.0f;
}

void AEliteKnight::RespawnCharacter()
{
	Super::RespawnCharacter();
	MonsterController->BossUI->SetHP(1);
	MonsterController->BossUI->RemoveFromParent();
}

float AEliteKnight::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsBoss)
	{
		MonsterDataStruct.CharacterHp -= DamageAmount;

		MonsterController->BossUI->DecreaseHPGradual(this, MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp);
		MonsterController->BossUI->SetDamageText(DamageAmount);
	}

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return 0.0f;
}

