// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\Boss_1\BossAIController.h"
#include "..\Player\PlayerCharacter.h"
#include "..\Boss_1\JesusBoss.h"

UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto Boss = Cast<AJesusBoss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));

	if (CurrentPawn == nullptr)
		return false;

	auto Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

	if (Target == nullptr)
		return false;

	if (Target->GetDistanceTo(CurrentPawn) <= AttackDistance && !Boss->IsStun)
	{
		Boss->CanAttack = true;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("CanAttack", Boss->CanAttack);
		return true;
	}
	else
	{
		Boss->CanAttack = false;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("CanAttack", Boss->CanAttack);
		return false;
	}
}
