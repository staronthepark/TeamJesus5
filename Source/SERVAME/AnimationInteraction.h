// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "BaseAnimInstance.h"
#include "AnimationInteraction.generated.h"

UCLASS()
class SERVAME_API AAnimationInteraction : public AInteractionActor
{
	GENERATED_BODY()

public:

	AAnimationInteraction();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* MeshComp;

	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginTriggerEvent()override;
	virtual void EndTriggerEvent()override;
	virtual void EnableEvent()override;
};