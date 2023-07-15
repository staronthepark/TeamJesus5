// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeAction.h"

UBTTask_ChangeAction::UBTTask_ChangeAction()
{
}

EBTNodeResult::Type UBTTask_ChangeAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Boss = Cast<AJesusBoss>(OwnerComp.GetAIOwner()->GetPawn());
	Boss->ChangeAnimType(BossAnimationType::RUN);
	return EBTNodeResult::Succeeded;
}



