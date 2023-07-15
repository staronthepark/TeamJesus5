// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_LockOn.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBTDecorator_LockOn : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_LockOn();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
