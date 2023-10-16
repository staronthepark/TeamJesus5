// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\KinghtMonster.h"
#include "DeadBodyOfKinght.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API ADeadBodyOfKinght : public AKinghtMonster
{
	GENERATED_BODY()
	
public:
	ADeadBodyOfKinght();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void RespawnCharacter() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Trigger;

	UFUNCTION()
	void OnTriggerBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
