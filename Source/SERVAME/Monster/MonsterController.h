// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "MonsterController.generated.h"


UCLASS()
class SERVAME_API AMonsterController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterController();

	virtual void BeginPlay();

	bool IsArrived = false;
	bool FindPlayer = false;

	void Movement(FVector Location);
	void MoveWhenArrived(FVector Location);

	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnTargetPerceptionUpdated_Delegate(AActor* Actor, FAIStimulus Stimulus);

	class AEnemyMonster* Monster;
	class APlayerCharacter* Player;

	UPROPERTY(EditAnywhere)
	float PerceptionSight = 0.f;

	UPROPERTY()
	UAISenseConfig_Sight* Sight;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

	TArray<AActor*> DetectedActorArr;

private:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void Tick(float DeltaTime) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;

};