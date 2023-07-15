// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanRangeAttack.h"
#include "..\Player\PlayerCharacter.h"
#include "..\Boss_1\JesusBoss.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CanRangeAttack::UBTDecorator_CanRangeAttack()
{
	NodeName = TEXT("CanRangeAttack");
}

bool UBTDecorator_CanRangeAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto Boss = Cast<AJesusBoss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));
	if (CurrentPawn == nullptr)
		return false;

	auto Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return false;

	if (Target->GetDistanceTo(CurrentPawn) >= AttackDistance && !Boss->IsStun)
	{
		Boss->CanAttack = true;
		return true;
	}
	else
	{
		Boss->CanAttack = false;
		return false;
	}
}
