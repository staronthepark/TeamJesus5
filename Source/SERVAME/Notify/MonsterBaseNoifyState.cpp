// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBaseNoifyState.h"

std::tuple<ANunMonster*, AJamsig*, AKinghtMonster*> UMonsterBaseNoifyState::GetMonster(USkeletalMeshComponent* MeshComp)
{
	return std::tuple<ANunMonster*, AJamsig*, AKinghtMonster*>(
		Cast<ANunMonster>(MeshComp->GetOwner()),
		Cast<AJamsig>(MeshComp->GetOwner()),
		Cast<AKinghtMonster>(MeshComp->GetOwner()));
}
