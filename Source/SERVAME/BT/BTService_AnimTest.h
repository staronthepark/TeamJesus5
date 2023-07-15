// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "..\Boss_1\JesusBoss.h"
#include "BTService_AnimTest.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBTService_AnimTest : public UBTService
{
	GENERATED_BODY()
	UBTService_AnimTest();

	AJesusBoss* Boss;

	UPROPERTY(EditAnywhere, Category = "True == Play / False == Stop")
	bool IsPlay;

	UPROPERTY(EditAnywhere, Category = "Test Anim Type")
	TEnumAsByte<BossAnimationType> TestAnimType;

	TMap<BossAnimationType, TMap<bool, TFunction<void()>>>TestAnimMap;
	UAnimMontage* TestMontage;
	bool IsPlayed = false;

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
