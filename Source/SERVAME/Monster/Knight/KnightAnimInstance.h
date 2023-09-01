// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\..\BaseAnimInstance.h"
#include "KnightAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInterpStart);
DECLARE_MULTICAST_DELEGATE(FOnInterpEnd);
DECLARE_MULTICAST_DELEGATE(FOnKnockBackStart);
DECLARE_MULTICAST_DELEGATE(FOnKnockBackEnd);
DECLARE_MULTICAST_DELEGATE(FOnSpawningBegin);
DECLARE_MULTICAST_DELEGATE(FOnSpawningEnd);

UCLASS()
class SERVAME_API UKnightAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

	UFUNCTION()
	void AnimNotify_InterpStart();

	UFUNCTION()
	void AnimNotify_InterpEnd();

	UFUNCTION()
	void AnimNotify_KnockBackStart();
		
	UFUNCTION()
	void AnimNotify_KnockBackEnd();

	UFUNCTION()
	void AnimNotify_SpawningBegin();

	UFUNCTION()
	void AnimNotify_SpawningEnd();
public:
	UKnightAnimInstance();	
	
	FOnInterpStart InterpStart;
	FOnInterpEnd InterpEnd;
	FOnKnockBackStart KnockBackStart;
	FOnKnockBackEnd KnockBackEnd;
	FOnSpawningBegin SpawningBegin;
	FOnSpawningEnd SpawningEnd;
};
