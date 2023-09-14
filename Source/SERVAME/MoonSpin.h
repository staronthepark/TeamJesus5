// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoonSpin.generated.h"

UCLASS()
class SERVAME_API AMoonSpin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoonSpin();

	UPROPERTY(Editanywhere)
		UStaticMeshComponent* StaticMeshComp1;

	UPROPERTY(Editanywhere)
		float RotateSpeed;

	float Rotation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
