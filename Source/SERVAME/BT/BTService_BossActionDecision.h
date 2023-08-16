// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "..\Boss_1\JesusBoss.h"
#include "..\Player\PlayerCharacter.h"
#include "Engine/DataTable.h"
#include "BTService_BossActionDecision.generated.h"

UCLASS()
class SERVAME_API UBTService_BossActionDecision : public UBTService
{
	GENERATED_BODY()

	AJesusBoss* Boss;
	APlayerCharacter* Target;

	float BossToPlayer;
	bool IsExcute = false;
	float BossSpeed;

	const UEnum* BossActionEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("BossActionType"), true);

	UPROPERTY(EditAnywhere, Category = "Boss Pattern")
	TArray<TEnumAsByte<BossActionType>> BossPatternArr;

	UPROPERTY(EditAnywhere, Category = "Boss HalfHp Pattern")
	TArray<TEnumAsByte<BossActionType>> BossHalfHpPatternArr;

	UPROPERTY(EditAnywhere, Category = "Boss Range Pattern")
	TArray<TEnumAsByte<BossActionType>> BossRangePatternArr;

	UPROPERTY(EditAnywhere, Category = "Boss FollowUp Pattern")
	TArray<TEnumAsByte<BossActionType>> BossFollowUpPatternArr;

	UPROPERTY(EditAnywhere, Category = "Boss Step Pattern")
	TArray<TEnumAsByte<BossActionType>> BossStepPatternArr;

public:
	UBTService_BossActionDecision();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void DecisionBossAction(UBehaviorTreeComponent& OwnerComp);
	void CheckCanMove(UBehaviorTreeComponent& OwnerComp);
	void PushAction(TArray<TEnumAsByte<BossActionType>> ActionArr);

	//BossActionTemp SetAction(UBehaviorTreeComponent& OwnerComp);
	//void OnCoolTimeFinished(BossActionTemp Temp);
	//void FirstSetAction();
};
