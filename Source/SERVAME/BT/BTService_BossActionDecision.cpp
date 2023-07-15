// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_BossActionDecision.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_BossActionDecision::UBTService_BossActionDecision()
{
	NodeName = TEXT("BossActionDecision");
}

void UBTService_BossActionDecision::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (Boss == nullptr)
		Boss = Cast<AJesusBoss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));
	if (Target == nullptr)
		Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

	auto CalcDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName(TEXT("CalcDist")));

	if (Boss != nullptr && Target != nullptr)
		BossToPlayer = FVector::Distance(Boss->GetActorLocation(), Target->GetActorLocation());

	if (Boss != nullptr)
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossCurrentStatus")), Boss->CurrentStatus);
		
	if (CalcDistance >= Boss->MaxAtkRange)
	{
		Boss->BossSuperAction = SUPER_MOVE;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossBaseAction")), Boss->BossSuperAction);
	}
	else if(CalcDistance <= Boss->MaxAtkRange && !Boss->ChangeSuperAction)
	{
		Boss->BossSuperAction = SUPER_ATTACK;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossBaseAction")), Boss->BossSuperAction);
	}
	
	BossSpeed = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("Speed");

	DecisionBossAction(OwnerComp);
	CheckCanMove(OwnerComp);
}

void UBTService_BossActionDecision::DecisionBossAction(UBehaviorTreeComponent& OwnerComp)
{
	if (!IsExcute)
	{
		PushAction(BossPatternArr);
		PushAction(BossRangePatternArr);
		PushAction(BossFollowUpPatternArr);

		Boss->CurrentActionTemp = Boss->MeleeActionArr[0];
		IsExcute = true;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossActionType")), Boss->CurrentActionTemp.ActionType);
	}
	if (!Boss->Push2PhasePattern && Boss->IsHalfHp)
	{
		PushAction(BossHalfHpPatternArr);
		Boss->Push2PhasePattern = true;
	}
}

void UBTService_BossActionDecision::CheckCanMove(UBehaviorTreeComponent& OwnerComp)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("CanMove")), Boss->CanMove);
}

void UBTService_BossActionDecision::PushAction(TArray<TEnumAsByte<BossActionType>> ActionArr)
{
	if (ActionArr.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Some Array is Empty!!!"));
		return;
	}

	BossActionTemp ActionTemp{};

	for (int i = 0; i < ActionArr.Num(); i++)
	{
		auto BossActionName = BossActionEnum->GetNameStringByValue(ActionArr[i]);
		UE_LOG(LogTemp, Warning, TEXT("PushAction : %s"), *BossActionName);

		ActionTemp.ActionType = Boss->GetActionData(*BossActionName)->ActionType;
		ActionTemp.ActionDirection = Boss->GetActionData(*BossActionName)->ActionDirectionType;
		ActionTemp.Percentage = Boss->GetActionData(*BossActionName)->Percentage;
		ActionTemp.SuperAction = Boss->GetActionData(*BossActionName)->SuperAction;
		ActionTemp.Speed = Boss->GetActionData(*BossActionName)->Speed;
		ActionTemp.AttackType = Boss->GetActionData(*BossActionName)->AttackType;
		ActionTemp.CanParring = Boss->GetActionData(*BossActionName)->CanParring;
		ActionTemp.CanContinuity = Boss->GetActionData(*BossActionName)->CanContinuity;
		ActionTemp.HitCancel = Boss->GetActionData(*BossActionName)->HitCancel;
		ActionTemp.Distance = Boss->GetActionData(*BossActionName)->Distance;

		Boss->AddArrMap[ActionTemp.AttackType](&ActionTemp);
	}
}


