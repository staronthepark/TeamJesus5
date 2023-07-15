// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SpeeUp.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBTTask_SpeeUp : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_SpeeUp();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
};
