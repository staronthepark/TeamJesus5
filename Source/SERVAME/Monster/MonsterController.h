// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "..\UI\BossUI.h"
#include "MonsterController.generated.h"

UCLASS()
class SERVAME_API AMonsterController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterController();

	UPROPERTY()
	UBossUI* BossUI;
	UPROPERTY()
	TSubclassOf<UBossUI> BossUIClass;

	virtual void BeginPlay();

	bool IsArrived = false;
	bool FindPlayer = false;
	bool CanPerception = false;
	TAtomic<bool> IsUIActivate = false;

	void Movement(FVector Location);
	void MoveWhenArrived(FVector Location);
	void Patrol(FVector Location, int PatrolArrNum);
	void MoveToStartLoc(FVector location);

	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION()
	void ChangeLanguage();

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

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
private:
	virtual void Tick(float DeltaTime) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;

};