// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AddWalkCount.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AddWalkCount::UBTTask_AddWalkCount()
{
	NodeName = "AddWalkCount";
}

EBTNodeResult::Type UBTTask_AddWalkCount::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Boss = Cast<AJesusBoss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));
	Boss->CurrentWalkCount++;
	return EBTNodeResult::Type();
}
