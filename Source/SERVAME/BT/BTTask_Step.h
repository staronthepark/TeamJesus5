// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "..\Boss_1\JesusBoss.h"
#include "BTTask_Step.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBTTask_Step : public UBTTaskNode
{
	GENERATED_BODY()

	AJesusBoss* Boss;
public:
	UBTTask_Step();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	void DoRandomStep();
};
