// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Confrontation.h"
#include "..\Boss_1\BossAIController.h"
#include "..\Boss_1\BossAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_Confrontation::UBTService_Confrontation()
{
	NodeName = TEXT("Confrontation");
}

void UBTService_Confrontation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	Boss = Cast<AJesusBoss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));
	Boss->BossAnimInstance = Cast<UBossAnimInstance>(Boss->GetMesh()->GetAnimInstance());

	if (Boss->DoStep)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Boss->GetWorld());
		if (NavSystem == nullptr)
			return;

		FNavLocation RandomLocation;

		if (NavSystem->GetRandomPointInNavigableRadius(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), MaxRadius, RandomLocation))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("OriginPos")), RandomLocation.Location);
			auto Loc = RandomLocation.Location * GetRandomDirection();

			if (Loc == FVector::ZeroVector)
				return;

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("ConfrontationPos")), Loc);
			UE_LOG(LogTemp, Log, TEXT("EBTNodeResult::Succeeded;"));
		}

		auto BossVelocity = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName(TEXT("Speed")));
	}
}

void UBTService_Confrontation::DrawLocation(FNavLocation RandomPos)
{
	DrawDebugSphere(GetWorld(), RandomPos, 50, 20, FColor::Green, false, 1.0f);
}

FVector UBTService_Confrontation::GetRandomDirection()
{
	srand(time(NULL));

	int DirectionNum = (rand() % 3) + 2;

	switch (DirectionNum)
	{
	case 0:
		Boss->ChangeAnimType(BossAnimationType::RUN);
		return FVector::ForwardVector;
	case 1:		
		Boss->ChangeAnimType(BossAnimationType::RUN_L);
		return FVector::LeftVector;
	case 2:		
		Boss->ChangeAnimType(BossAnimationType::RUN_R);
		return FVector::RightVector;
	}

	DirNum = DirectionNum;


	return FVector::ZeroVector;
}
