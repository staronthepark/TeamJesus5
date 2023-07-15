// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_2_CanAttack.h"
#include "..\Player\PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\Boss_2\Boss2AIController.h"
#include "..\Boss_2\JesusBoss2.h"

UBTDecorator_2_CanAttack::UBTDecorator_2_CanAttack()
{
}

bool UBTDecorator_2_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto Boss2 = Cast<AJesusBoss2>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));

	if (CurrentPawn == nullptr)
		return false;

	auto Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

	if (Target == nullptr)
		return false;

	if (Target->GetDistanceTo(CurrentPawn) <= AttackDistance && !Boss2->IsStun)
	{
		Boss2->CanAttack = true;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("CanAttack", Boss2->CanAttack);
		return true;
	}
	else
	{
		Boss2->CanAttack = false;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("CanAttack", Boss2->CanAttack);
		return false;
	}
}
