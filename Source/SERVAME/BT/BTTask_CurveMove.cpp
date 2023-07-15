// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CurveMove.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_CurveMove::UBTTask_CurveMove()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_CurveMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Boss = Cast<AJesusBoss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));
	Player = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

	LocationArr.SetNum(Boss->Spline->GetNumberOfSplinePoints());

	if (!test)
	{
		for (int32 i = 0; i < Boss->Spline->GetNumberOfSplinePoints(); ++i)
		{
			FVector Location = Boss->Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
			LocationArr[i] = Location;
			UE_LOG(LogTemp, Warning, TEXT("Point %d Location: X=%f, Y=%f, Z=%f"), i, Location.X, Location.Y, Location.Z);
		}

		test = true;
	}

	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (count == Boss->Spline->GetNumberOfSplinePoints())
		return EBTNodeResult::Succeeded;

	auto DistanceToSpline = FVector::Distance(Boss->GetActorLocation(), LocationArr[count]);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), DistanceToSpline);

	if (DistanceToSpline <= 600.f)
	{
		Boss->GetCharacterMovement()->MaxWalkSpeed = testspeed;
		AIController->MoveToLocation(LocationArr[count]);
		count++;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_CurveMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTTask_CurveMove::Test(UBehaviorTreeComponent& OwnerComp)
{
	FVector StartPos = Boss->GetActorLocation();
	FVector EndPos = Player->GetActorLocation();


}
