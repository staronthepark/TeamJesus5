// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "StaticMeshCollision.generated.h"

UENUM(BlueprintType)
enum class EMaterialType : uint8
{
	NONE,
	BLOOD,
	STONE,
	WOOD,
	STEAL,
};

UCLASS()
class SERVAME_API AStaticMeshCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	AStaticMeshCollision();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMaterialType MaterialType;

	FTimerHandle TimeHandle;

	TArray<UPrimitiveComponent*>CompArray;

protected:
	virtual void BeginPlay() override;

	void SpawnEffect();

	virtual void OverlapBegin(UPrimitiveComponent* OtherComp);
	virtual void OverlapEnd(UPrimitiveComponent* OtherComp);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};