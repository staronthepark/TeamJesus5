// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMetaData.h"
#include "Boss2AnimInstance.h"
#include "Boss2AnimMetaData.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBoss2AnimMetaData : public UAnimMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	Boss2AnimationType AnimType = Boss2AnimationType::NONE;
};
