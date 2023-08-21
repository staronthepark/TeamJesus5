// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AIPerceptionComponent.h"
#include "Engine/DataTable.h"
#include "Templates/Atomic.h"
#include "..\UI\BossUI.h"
#include "BossAIController.generated.h"
/**
 * 
 */

UCLASS()
class SERVAME_API ABossAIController : public AAIController
{
	GENERATED_BODY()
public:
	ABossAIController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	UBossUI* BossUI;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void ChangeLanguage();

	UPROPERTY(BlueprintReadWrite)
	class AJesusBoss* Boss;

	UPROPERTY()
	class UAISenseConfig_Sight* Sight;

	TArray<AActor*> DetectedActorArr;

	TAtomic<bool> IsUIActivate = false;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TSubclassOf<UBossUI> BossUIClass;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, Category = AI)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = AI)
	UBlackboardData* BlackboardData;

};
