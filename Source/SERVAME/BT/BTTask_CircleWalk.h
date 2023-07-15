// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "..\Boss_1\JesusBoss.h"
#include "..\Player\PlayerCharacter.h"
#include "BTTask_CircleWalk.generated.h"

/**
 * 
 */	
UCLASS()
class SERVAME_API UBTTask_CircleWalk : public UBTTaskNode
{
	GENERATED_BODY()

	AJesusBoss* Boss;
	APlayerCharacter* Player;

	FVector CircleCenter;
	float DistanceToPlayer;

	int32 count = 0;	

	UPROPERTY(EditAnywhere,Category = "Num of Segments")
	int32 NumSegments = 8; // 원을 구성하는 선분의 수

	UPROPERTY(EditAnywhere, Category = "-1 to Num of Segments")
	int32 reverseCount = 7;

	UPROPERTY(EditAnywhere, Category = "Circle Range")
	float OriginCircleRange = 1000.f;

	float CircleRange;
	TArray<FVector> CirclePoints;
	//bool RandomBool;
	float StartIndex = 0;

public:
	UBTTask_CircleWalk();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	void DrawCircleAroundLocation(UBehaviorTreeComponent& OwnerComp,const FVector& Center, float Radius);
};
