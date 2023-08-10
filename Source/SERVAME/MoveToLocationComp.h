// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveToLocationComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERVAME_API UMoveToLocationComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMoveToLocationComp();

	UPROPERTY(Editanywhere)
		float MoveSpeed;

	void SetTargetLocation(FVector Location);

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	AActor* Owner;
	FVector TargetLocation;
};
