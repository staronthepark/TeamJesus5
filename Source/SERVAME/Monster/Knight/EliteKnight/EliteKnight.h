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

	UPROPERTY(EditAnyWhere, Category = "Sprint")
	float SprintAttackRadius = 100.f;
	float SprintDuration = 1.f;
	float SprintTime;
	float BlendSpeed;
	float BlendDirection;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
