// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_2_SearchTarget.h"
#include "..\Boss_2\Boss2AnimInstance.h"
#include "..\Player\PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_2_SearchTarget::UBTService_2_SearchTarget()
{
	NodeName = TEXT("SearchTarget");
}

void UBTService_2_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	Boss2 = Cast<AJesusBoss2>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));
	Boss2->Boss2AnimInstance = Cast<UBoss2AnimInstance>(Boss2->GetMesh()->GetAnimInstance());
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName(TEXT("Speed")), Boss2->Boss2AnimInstance->Speed);

	if (Boss2 == nullptr)
		return;

	auto Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Player);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PlayerPos")), Player->GetActorLocation());

		if (Boss2->BossDataStruct.CharacterHp <= 0)
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsDead")), true);

		if (Player->PlayerDataStruct.CharacterHp <= 0)
		{
			Boss2->IsPlayerAlive = false;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsPlayerAlive")), false);
		}
		else
		{
			Boss2->IsPlayerAlive = true;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsPlayerAlive")), true);
		}
		FindPlayerPos(OwnerComp);
	}
}

void UBTService_2_SearchTarget::FindPlayerPos(UBehaviorTreeComponent& OwnerComp)
{
	auto Player = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

	if (Player == nullptr)
		return;

	auto TargetLoc = Player->GetActorLocation() - Boss2->GetActorLocation();
	FVector Forward = Boss2->GetActorForwardVector();
	FVector Right = Boss2->GetActorRightVector();
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
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), B2_FOWARD);
				Boss2->PlayerDirection = B2_FOWARD;
			}
			else
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : B2_LEFT"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), B2_LEFT);
				Boss2->PlayerDirection = B2_LEFT;
			}
		}

		if (RightSpeed > 0)
		{
			if (ForwardSpeed > RightSpeed)
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : Forward"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), B2_FOWARD);
				Boss2->PlayerDirection = B2_FOWARD;
			}
			else
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : B2_RIGHT"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), B2_RIGHT);
				Boss2->PlayerDirection = B2_RIGHT;
			}
		}
	}
	else if (ForwardSpeed < 0)
	{
		if (RightSpeed < 0)
		{
			if (-ForwardSpeed > -RightSpeed)
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : B2_BACK"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), B2_BACK);
				Boss2->PlayerDirection = B2_BACK;
			}
			else
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : B2_LEFT"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), B2_LEFT);
				Boss2->PlayerDirection = B2_LEFT;
			}
		}
		if (RightSpeed > 0)
		{
			if (-ForwardSpeed > RightSpeed)
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : B2_BACK"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), B2_BACK);
				Boss2->PlayerDirection = B2_BACK;
			}
			else
			{
				//UE_LOG(LogTemp, Log, TEXT("DotProduct : B2_RIGHT"));
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Direction")), B2_RIGHT);
				Boss2->PlayerDirection = B2_RIGHT;
			}
		}
	}
}
