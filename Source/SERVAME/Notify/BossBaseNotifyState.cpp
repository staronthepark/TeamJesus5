// Fill out your copyright notice in the Description page of Project Settings.


#include "BossBaseNotifyState.h"

std::tuple<AJesusBoss*, AJesusBoss2*> UBossBaseNotifyState::GetBoss(USkeletalMeshComponent* MeshComp)
{
	return std::tuple<AJesusBoss*, AJesusBoss2*>(Cast<AJesusBoss>(MeshComp->GetOwner()), Cast<AJesusBoss2>(MeshComp->GetOwner()));
}
