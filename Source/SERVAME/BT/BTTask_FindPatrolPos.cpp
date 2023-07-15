// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\Boss_1\JesusBoss.h"
#include "..\Boss_1\BossAnimInstance.h"
#include "..\Boss_1\BossAIController.h"
#include "NavigationSystem.h"
#include "BluePrint/AIBlueprintHelperLibrary.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Boss = Cast<AJesusBoss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));
	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (Boss == nullptr)
		return EBTNodeResult::Failed;
	else
		Boss->BossAnimInstance = Cast<UBossAnimInstance>(Boss->GetMesh()->GetAnimInstance());

	if (CurrentPawn == nullptr)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Boss->GetWorld());
	if (NavSystem == nullptr)
		return EBTNodeResult::Failed;

	FNavLocation RandomLocation;

	//UE_LOG(LogTemp, Log, TEXT("Boss->AnimInstance->IsStart, %s"),
	//	Boss->BossAnimInstance->IsStart ? TEXT("TRUE") : TEXT("FALSE"));


	if (NavSystem->GetRandomPointInNavigableRadius(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), 500.f, RandomLocation))
	{
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PatrolPos")), RandomLocation.Location);
		Boss->BossAnimInstance->Vertical = 1;
		Boss->BossAnimInstance->Horizontal = 0;

		UE_LOG(LogTemp, Log, TEXT("EBTNodeResult::Succeeded;"));

		return EBTNodeResult::Succeeded;
	}

	UE_LOG(LogTemp, Error, TEXT("EBTNodeResult::Failed;"));
	return EBTNodeResult::Failed;
}
