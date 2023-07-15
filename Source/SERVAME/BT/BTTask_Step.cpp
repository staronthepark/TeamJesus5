// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Step.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\Boss_1\BossAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"

UBTTask_Step::UBTTask_Step()
{
	NodeName = TEXT("Step");
}

EBTNodeResult::Type UBTTask_Step::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Boss = Cast<AJesusBoss>(OwnerComp.GetAIOwner()->GetPawn());

	//auto Type = Boss->GetTypeFromMetaData(Boss->GetCurrentMontage());

	//if(Type  == BossAnimationType::BACKSTEP || Type == BossAnimationType::RIGHTSTEP || Type == BossAnimationType::LEFTSTEP)
	//	return EBTNodeResult::Failed;
		
	auto Name = Boss->StartMontage->GetName();

	if (Name == "AM_BossBackStep" || Name == "AM_BossLeftStep" || Name == "AM_BossRightStep")
		return EBTNodeResult::Failed;


	DoRandomStep();
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsStep")), Boss->DoStep);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossBaseAction")), SUPER_MOVE);
	return EBTNodeResult::Succeeded;
}

void UBTTask_Step::DoRandomStep()
{
	srand(time(NULL));

	switch (rand() % 3)
	{
	case 0:
		Boss->BossAnimInstance->PlayMontage(Boss->BossMontageMap[BossAnimationType::BACKSTEP]);
		break;
	case 1:
		Boss->BossAnimInstance->PlayMontage(Boss->BossMontageMap[BossAnimationType::RIGHTSTEP]);
		break;
	case 2:
		Boss->BossAnimInstance->PlayMontage(Boss->BossMontageMap[BossAnimationType::LEFTSTEP]);
		break;

	}
}


