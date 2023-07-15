// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "..\Boss_1\BossAIController.h"
#include "..\Boss_1\JesusBoss.h"
#include "..\Player\PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Boss = Cast<AJesusBoss>(OwnerComp.GetAIOwner()->GetPawn());

	if (Boss == nullptr)
		return EBTNodeResult::Failed;

	if (Boss->IsStun)
		return EBTNodeResult::Failed;

	if(Boss->DoStep)
		return EBTNodeResult::Failed;

	auto BossAction = OwnerComp.GetBlackboardComponent()->GetValueAsEnum(FName(TEXT("BossActionType")));

	Dist = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName(TEXT("CalcDist")));
	
	if (Boss->BossAnimInstance->IsStart && !Boss->IsAttacking && !Boss->DoStep && Boss->IsAttackMontageEnd && !Boss->IsParriged)
	{
		Boss->BossSuperAction = SUPER_ATTACK;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossBaseAction")), Boss->BossSuperAction);
		Boss->BossAttackMap[static_cast<BossActionType>(BossAction)](Boss);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (Boss->IsAttacking == false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	else if (Boss->IsStun)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	else
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
