// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchTarget.h"
#include "..\Boss_1\JesusBoss.h"
#include "..\Boss_1\BossAnimInstance.h"
#include "..\Player\PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("SearchTarget");
}

void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	Boss = Cast<AJesusBoss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));
	Boss->BossAnimInstance = Cast<UBossAnimInstance>(Boss->GetMesh()->GetAnimInstance());
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName(TEXT("Speed")), Boss->BossAnimInstance->Speed);

	if (Boss == nullptr)
		return;

	//범위로 탐지
	/*auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return;

	UWorld* World = CurrentPawn->GetWorld();
	FVector Center = CurrentPawn->GetActorLocation();

	if (World == nullptr)
		return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(Boss->DetectRange),
		QueryParams);

	DrawDebugSphere(World, Center, Boss->DetectRange, 16, FColor::Blue, false, 0.2f);

	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(OverlapResult.GetActor());

			if (Player && Player->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Player);
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsLockOn")), true);

				DrawDebugSphere(World, Center, Boss->DetectRange, 16, FColor::Green, false, 0.2f);
				Boss->DetectRange = 3000.f;
				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsLockOn")), false);
		DrawDebugSphere(World, Center, Boss->DetectRange, 16, FColor::Red, false, 0.2f);
	}
	*/

	//Debug용
	//auto TargetLoc = DebugFunc(OwnerComp) - Boss->GetActorLocation();

	auto Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Player);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PlayerPos")), Player->GetActorLocation());

		if (Boss->BossDataStruct.CharacterHp <= 0)
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsDead")), true);

		if (Player->PlayerDataStruct.CharacterHp <= 0)
		{
			Boss->IsPlayerAlive = false;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsPlayerAlive")), false);
		}
		else
		{
			Boss->IsPlayerAlive = true;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsPlayerAlive")), true);
		}
		FindPlayerPos(OwnerComp);
	}
}
	
void UBTService_SearchTarget::FindPlayerPos(UBehaviorTreeComponent& OwnerComp)
{
	auto Player = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	
	if (Player == nullptr)
		return;

	auto TargetLoc = Player->GetActorLocation() - Boss->GetActorLocation();
	FVector Forward = Boss->GetActorForwardVector();
	FVector Right = Boss->GetActorRightVector();
	float ForwardSpeed = FVector::DotProduct(TargetLoc, Forward);
	float RightSpeed = FVector::DotProduct(TargetLoc, Right);

	//UE_LOG(LogTemp, Log, TEXT("[ForwardSpeed : %f] [RightSpeed : %f]"), ForwardSpeed, RightSpeed);

	if (ForwardSpeed > 0)
	{
		if (RightSpeed < 0)
		{
			if (ForwardSpeed > -RightSpeed)
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : Forward"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), FOWARD);
				Boss->PlayerDirection = FOWARD;
			}
			else
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : Left"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), LEFT);
				Boss->PlayerDirection = LEFT;
			}
		}

		if (RightSpeed > 0)
		{
			if (ForwardSpeed > RightSpeed)
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : Forward"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), FOWARD);
				Boss->PlayerDirection = FOWARD;
			}
			else
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : Right"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), RIGHT);
				Boss->PlayerDirection = RIGHT;
			}
		}
	}
	else if (ForwardSpeed < 0)
	{
		if (RightSpeed < 0)
		{
			if (-ForwardSpeed > -RightSpeed)
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : Back"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), BACK);
				Boss->PlayerDirection = BACK;
			}
			else
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : Left"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), LEFT);
				Boss->PlayerDirection = LEFT;
			}
		}
		if (RightSpeed > 0)
		{
			if (-ForwardSpeed > RightSpeed)
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : Back"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), BACK);
				Boss->PlayerDirection = BACK;
			}
			else
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : Right"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), RIGHT);
				Boss->PlayerDirection = RIGHT;
			}
		}
	}
}

FVector UBTService_SearchTarget::DebugFunc(UBehaviorTreeComponent& OwnerComp)
{
	for (TActorIterator<AStaticMeshActor> ActorIter(GetWorld()); ActorIter; ++ActorIter)
	{
		AStaticMeshActor* testActor = *ActorIter;

		if (testActor)
		{
			FString str = "testCube";
			FString str2 = testActor->GetName();

			if (str2 == str)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("testObj")), testActor);
				return testActor->GetActorLocation();
			}
		}
		else
			return FVector::ZeroVector;
	}

	return FVector::ZeroVector;
}
