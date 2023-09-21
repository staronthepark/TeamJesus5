// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\KinghtMonster.h"
#include "EliteKnight.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API AEliteKnight : public AKinghtMonster
{
	GENERATED_BODY()

public:
	AEliteKnight();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void RunBlending();

	UPROPERTY(EditAnyWhere, Category = "Sprint")
	float SprintDuration = 1.f;
	UPROPERTY(EditAnyWhere, Category = "Sprint")
	float SprintAttackRadius;
	float SprintTime;
	float BlendSpeed;

};
