// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KnightHeadAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UKnightHeadAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UKnightHeadAnimInstance();

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FRotator Rot;
	
};
