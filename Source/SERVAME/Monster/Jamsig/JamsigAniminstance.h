// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\..\BaseAnimInstance.h"
#include "JamsigAniminstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnKnockBackStart);
DECLARE_MULTICAST_DELEGATE(FOnKnockBackEnd);

UCLASS()
class SERVAME_API UJamsigAniminstance : public UBaseAnimInstance
{
	GENERATED_BODY()
public:
	UJamsigAniminstance();

	FOnKnockBackStart KnockBackStart;
	FOnKnockBackEnd KnockBackEnd;

	UFUNCTION()
	void AnimNotify_KnockBackStart();

	UFUNCTION()
	void AnimNotify_KnockBackEnd();

};
