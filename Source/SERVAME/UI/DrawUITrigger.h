// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\TriggerActor.h"
#include "..\Player\PlayerCharacter.h"
#include "DrawUITrigger.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API ADrawUITrigger : public ATriggerActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGuides DrawImageType;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool KeepActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TriggerOnlyOnce;

	APlayerCharacter* PlayerCharacter;

	FTimerHandle UITimerHandle;


	void DrawTimeEnd();
	
	void PauseGame();

	virtual void BeginPlay() override;
	virtual void BeginTriggerEvent() override;
	virtual void EndTriggerEvent() override;
	virtual void EnableEvent() override;
};