// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "..\Boss_2\JesusBoss2.h"
#include "..\Player\PlayerCharacter.h"
#include "Engine/DataTable.h"
#include "BTService_2_BossActionDecision.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBTService_2_BossActionDecision : public UBTService
{
	GENERATED_BODY()
	AJesusBoss2* Boss2;
	APlayerCharacter* Target;

	float BossToPlayer;
	float BossSpeed;
	bool IsExcute = false;

	const UEnum* Boss2ActionEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("Boss2ActionType"), true);

	UPROPERTY(EditAnywhere, Category = "Boss Pattern")
	TArray<TEnumAsByte<Boss2ActionType>> BossPatternArr;

	UPROPERTY(EditAnywhere, Category = "Boss Range Pattern")
	TArray<TEnumAsByte<Boss2ActionType>> BossRangePatternArr;

	UPROPERTY(EditAnywhere, Category = "Boss FollowUp Pattern")
	TArray<TEnumAsByte<Boss2ActionType>> BossFollowUpPatternArr;


public:
	UBTService_2_BossActionDecision();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void DecisionBossAction(UBehaviorTreeComponent& OwnerComp);
	void CheckCanMove(UBehaviorTreeComponent& OwnerComp);
	void PushAction(TArray<TEnumAsByte<Boss2ActionType>> ActionArr);
};
