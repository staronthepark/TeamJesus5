// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\KinghtMonster.h"
#include "PersistentKnight.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API APersistentKnight : public AKinghtMonster
{
	GENERATED_BODY()

public:
	APersistentKnight();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Die")
	FVector AddVec;
	bool IsFirstDie = true;
	bool Reviving = false;

	void FirstDie();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float Die(float Dm) override;
};
