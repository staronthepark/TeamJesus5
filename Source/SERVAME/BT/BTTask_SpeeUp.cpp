// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SpeeUp.h"
#include "..\Boss_1\JesusBoss.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_SpeeUp::UBTTask_SpeeUp()
{

}

EBTNodeResult::Type UBTTask_SpeeUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Boss = Cast<AJesusBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (Boss == nullptr)
		return EBTNodeResult::Failed;

	Boss->GetCharacterMovement()->MaxWalkSpeed = 1000.f;

	return Result;
}
