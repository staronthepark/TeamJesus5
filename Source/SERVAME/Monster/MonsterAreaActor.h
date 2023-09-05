// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "EnemyMonster.h"
#include "MonsterAreaActor.generated.h"

UCLASS()
class SERVAME_API AMonsterAreaActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterAreaActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Area;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AreaSize = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnemyMonster*> MonsterArr;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckMonster();
};
