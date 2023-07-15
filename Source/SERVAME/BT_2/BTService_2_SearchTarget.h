// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "..\Boss_2\JesusBoss2.h"
#include "BTService_2_SearchTarget.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBTService_2_SearchTarget : public UBTService
{
	GENERATED_BODY()
	AJesusBoss2* Boss2;

public:
	UBTService_2_SearchTarget();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void FindPlayerPos(UBehaviorTreeComponent& OwnerComp);
};
