// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatManager.h"
#include "..\BaseCharacter.h"
#include "../Player/PlayerCharacter.h"

UCombatManager* UCombatManager::Instance = nullptr;
UCombatManager::UCombatManager()
{
	Instance = this;
}

UCombatManager::~UCombatManager()
{
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
}

void UCombatManager::DeactivateCollider()
{
}

void UCombatManager::AddMonsterInfo(ABaseCharacter* character)
{
}