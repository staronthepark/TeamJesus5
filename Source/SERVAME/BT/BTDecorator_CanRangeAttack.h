// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CanRangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBTDecorator_CanRangeAttack : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Attack Distance")
	float AttackDistance;

public:
	UBTDecorator_CanRangeAttack();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
