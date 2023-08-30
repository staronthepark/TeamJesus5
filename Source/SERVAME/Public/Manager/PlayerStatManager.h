// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerStatManager.generated.h"

UENUM(BlueprintType)
enum class EPlayerStat : uint8
{
	STREANGH,
	STAMINA,
	HP,
	SHIELD,
};

USTRUCT(BlueprintType)
struct FStatInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 SoulCost;

	TFunction<void()> Func;
};

/**
 * 
 */
UCLASS()
class SERVAME_API UPlayerStatManager : public UObject
{
	GENERATED_BODY()

public:
	UPlayerStatManager();

	
};
