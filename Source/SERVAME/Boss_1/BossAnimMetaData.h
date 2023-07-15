// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMetaData.h"
#include "BossAnimInstance.h"
#include "BossAnimMetaData.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBossAnimMetaData : public UAnimMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	BossAnimationType AnimType = BossAnimationType::NONE;
};
