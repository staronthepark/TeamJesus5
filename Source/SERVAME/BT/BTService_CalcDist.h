// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "..\BaseCharacter.h"
#include "..\Boss_1\JesusBoss.h"
#include "..\Player\PlayerCharacter.h"
#include "BTService_CalcDist.generated.h"

/**
 * 
 */

UCLASS()
class SERVAME_API UBTService_CalcDist : public UBTService
{
	GENERATED_BODY()

	ABaseCharacter* Boss;
	FVector KeyValue;

	UPROPERTY(EditAnywhere, Category = "Only Position Value(Yellow)")
	FBlackboardKeySelector Key;

public:
	UBTService_CalcDist();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
