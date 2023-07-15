// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/BlackboardComponent.h"
#include "BTService_CalcDist.h"

UBTService_CalcDist::UBTService_CalcDist()
{
	NodeName = TEXT("CalcDist");
}

void UBTService_CalcDist::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Boss = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));
	KeyValue = OwnerComp.GetBlackboardComponent()->GetValueAsVector(Key.SelectedKeyName);
	auto Distance = FVector::Distance(Boss->GetActorLocation(), KeyValue);

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName(TEXT("CalcDist")), Distance);
}
