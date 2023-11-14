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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* KnightHeadSkeletalMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Die")
	FVector AddVec;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Die")
	float Strength = 100.f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Die")
	FRotator RotVal;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Die")
	FRotator AddVal;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Die")
	float RotateTickTime;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Die")
	float RotateTime;

	FTimerHandle HeadTimer;
	FTimerHandle EndTimer;

	bool IsFirstDie = true;
	//bool Reviving = false;

	void FirstDie();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float Die(float Dm) override;
};
