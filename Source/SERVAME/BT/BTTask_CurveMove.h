// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "..\Boss_1\JesusBoss.h"
#include "..\Player\PlayerCharacter.h"
#include "BTTask_CurveMove.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBTTask_CurveMove : public UBTTaskNode
{
	GENERATED_BODY()

	AJesusBoss* Boss;
	APlayerCharacter* Player;
	TArray<FVector> LocationArr;

	int count = 1;
	bool test = false;
	float testspeed = 5000000.f;

	int NumPoints = 8;
	float CurveVal = 100.f;

public:
	UBTTask_CurveMove();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	void Test(UBehaviorTreeComponent& OwnerComp);
};
