// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_2_Attack.h"
#include "..\Boss_2\Boss2AIController.h"
#include "..\Player\PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_2_Attack::UBTTask_2_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_2_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Boss2 = Cast<AJesusBoss2>(OwnerComp.GetAIOwner()->GetPawn());

	if (Boss2 == nullptr)
		return EBTNodeResult::Failed;

	if (Boss2->IsStun)
		return EBTNodeResult::Failed;

	auto BossAction = OwnerComp.GetBlackboardComponent()->GetValueAsEnum(FName(TEXT("Boss2ActionType")));

	Dist = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName(TEXT("CalcDist")));

	if (Boss2->Boss2AnimInstance->IsStart && !Boss2->IsAttacking && Boss2->IsAttackMontageEnd)
	{
		Boss2->Boss2SuperAction = B2_SUPER_ATTACK;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Boss2BaseAction")), Boss2->Boss2SuperAction);
		Boss2->BossAttackMap[static_cast<Boss2ActionType>(BossAction)](Boss2);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_2_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (Boss2->IsAttacking == false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	else
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
