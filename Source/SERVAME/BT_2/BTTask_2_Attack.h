// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Boss_2\JesusBoss2.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_2_Attack.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBTTask_2_Attack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_2_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	AJesusBoss2* Boss2;
	float Dist;

	UPROPERTY()
	const UEnum* Boss2ActionEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("Boss2ActionType"), true);
};
