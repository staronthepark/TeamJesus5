// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "..\Boss_1\JesusBoss.h"
#include "BTService_Confrontation.generated.h"

/**
 * 
 */

UCLASS()
class SERVAME_API UBTService_Confrontation : public UBTService
{
	GENERATED_BODY()

	AJesusBoss* Boss;

	UPROPERTY(EditAnywhere, Category = "RandomRange")
	float MaxRadius;

	int DirNum;

public:

	UBTService_Confrontation();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void DrawLocation(FNavLocation RandomPos);
	FVector GetRandomDirection();
};
