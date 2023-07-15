// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "..\Boss_1\JesusBoss.h"
#include "BTService_SearchTarget.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBTService_SearchTarget : public UBTService
{
	GENERATED_BODY()
	AJesusBoss* Boss;

public:
	UBTService_SearchTarget();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void FindPlayerPos(UBehaviorTreeComponent& OwnerComp);
	FVector DebugFunc(UBehaviorTreeComponent& OwnerComp);
};
