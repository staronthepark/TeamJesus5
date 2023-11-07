// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatManager.h"
#include "..\BaseCharacter.h"

UCombatManager* UCombatManager::Instance = nullptr;
UCombatManager::UCombatManager()
{
	Instance = this;
}

UCombatManager::~UCombatManager()
{
	HitMonsterInfoArray.Empty();
}

UCombatManager& UCombatManager::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<UCombatManager>();
	}
	return *Instance;
}

void UCombatManager::ActivateCollider()
{
	if (!HitMonsterInfoArray.IsEmpty())
	{
		for (int i = 0; i < HitMonsterInfoArray.Num(); i++)
		{
			if (HitMonsterInfoArray[i] != nullptr)
				HitMonsterInfoArray[i]->ActivateHitCollision();
		}
	}
}

void UCombatManager::DeactivateCollider()
{
	if (!HitMonsterInfoArray.IsEmpty())
	{
		for (int i = 0; i < HitMonsterInfoArray.Num(); i++)
		{
			if (HitMonsterInfoArray[i] != nullptr)
				HitMonsterInfoArray[i]->DeactivateHitCollision();
		}
	}
}

void UCombatManager::AddMonsterInfo(ABaseCharacter* character)
{
	MonsterInfoArray.AddUnique(character);
	HitMonsterInfoArray.AddUnique(character);
	if (!MonsterInfoMap.Contains(character->MapName))
	{
		MonsterInfoMap.Add(character->MapName, TArray<ABaseCharacter*>());
	}
	MonsterInfoMap[character->MapName].AddUnique(character);
}