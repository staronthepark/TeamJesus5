// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NunMonster.h"
#include "NunTeleportActor.generated.h"

UCLASS()
class SERVAME_API ANunTeleportActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANunTeleportActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LocationActor;

	ANunMonster* Nun;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
