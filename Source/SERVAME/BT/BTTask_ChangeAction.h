// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "..\Boss_1\JesusBoss.h"
#include "BTTask_ChangeAction.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBTTask_ChangeAction : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ChangeAction();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
};
