// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AIPerceptionComponent.h"
#include "Engine/DataTable.h"
#include "Boss2AIController.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API ABoss2AIController : public AAIController
{
	GENERATED_BODY()
public:
	ABoss2AIController(const FObjectInitializer& ObjectInitializer);

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(BlueprintReadWrite)
	class AJesusBoss2* Boss2;

	UPROPERTY()
	class UAISenseConfig_Sight* Sight;

	TArray<AActor*> DetectedActorArr;

	//애니메이션 테스트 용 나중에 지우기.
	bool TestBool = false;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

private:
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, Category = AI)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = AI)
	UBlackboardData* BlackboardData;
};
