// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_2_BossActionDecision.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_2_BossActionDecision::UBTService_2_BossActionDecision()
{
	NodeName = TEXT("Boss2ActionDecision");
}

void UBTService_2_BossActionDecision::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (Boss2 == nullptr)
		Boss2 = Cast<AJesusBoss2>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));
	if (Target == nullptr)
		Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

	auto CalcDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName(TEXT("CalcDist")));


	if (Boss2 != nullptr && Target != nullptr)
		BossToPlayer = FVector::Distance(Boss2->GetActorLocation(), Target->GetActorLocation());

	if (CalcDistance >= Boss2->MaxAtkRange)
	{
		Boss2->Boss2SuperAction = B2_SUPER_MOVE;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossBaseAction")), Boss2->Boss2SuperAction);
	}
	else if (CalcDistance <= Boss2->MaxAtkRange && !Boss2->ChangeSuperAction)
	{
		Boss2->Boss2SuperAction = B2_SUPER_ATTACK;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Boss2BaseAction")), Boss2->Boss2SuperAction);
	}

	BossSpeed = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("Speed");

	DecisionBossAction(OwnerComp);
	CheckCanMove(OwnerComp);
}

void UBTService_2_BossActionDecision::DecisionBossAction(UBehaviorTreeComponent& OwnerComp)
{
	if (!IsExcute)
	{
		PushAction(BossPatternArr);
		PushAction(BossRangePatternArr);
		PushAction(BossFollowUpPatternArr);

		Boss2->CurrentActionTemp = Boss2->MeleeActionArr[0];
		IsExcute = true;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Boss2ActionType")), Boss2->CurrentActionTemp.ActionType);
	}
}

void UBTService_2_BossActionDecision::CheckCanMove(UBehaviorTreeComponent& OwnerComp)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("CanMove")), Boss2->CanMove);
}

void UBTService_2_BossActionDecision::PushAction(TArray<TEnumAsByte<Boss2ActionType>> ActionArr)
{
	if (ActionArr.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Some Array is Empty!!!"));
		return;
	}

	Boss2ActionTemp ActionTemp{};

	for (int i = 0; i < ActionArr.Num(); i++)
	{
		auto Boss2ActionName = Boss2ActionEnum->GetNameStringByValue(ActionArr[i]);
		UE_LOG(LogTemp, Warning, TEXT("PushAction : %s"), *Boss2ActionName);

		ActionTemp.ActionType = Boss2->GetActionData(*Boss2ActionName)->ActionType;
		ActionTemp.ActionDirection = Boss2->GetActionData(*Boss2ActionName)->ActionDirectionType;
		ActionTemp.Percentage = Boss2->GetActionData(*Boss2ActionName)->Percentage;
		ActionTemp.SuperAction = Boss2->GetActionData(*Boss2ActionName)->SuperAction;
		ActionTemp.Speed = Boss2->GetActionData(*Boss2ActionName)->Speed;
		ActionTemp.AttackType = Boss2->GetActionData(*Boss2ActionName)->AttackType;
		ActionTemp.CanParring = Boss2->GetActionData(*Boss2ActionName)->CanParring;
		ActionTemp.CanContinuity = Boss2->GetActionData(*Boss2ActionName)->CanContinuity;
		ActionTemp.HitCancel = Boss2->GetActionData(*Boss2ActionName)->HitCancel;
		ActionTemp.Distance = Boss2->GetActionData(*Boss2ActionName)->Distance;
		
		ActionTemp.TurnHead = Boss2->GetActionData(*Boss2ActionName)->TurnHead;

		Boss2->AddArrMap[ActionTemp.AttackType](&ActionTemp);
	}
}