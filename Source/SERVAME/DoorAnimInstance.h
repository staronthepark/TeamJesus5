// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "DoorAnimInstance.generated.h"

UENUM(BlueprintType)
enum class DoorAnimationType : uint8
{
	NONE,
	OPEN,
	CLOSE,
	KEEPOPEN,
};

UCLASS()
class SERVAME_API UDoorAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

public:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MyInstance, meta = (AllowPrivateAccess = "true"))
		DoorAnimationType DoorAnimationType;
	
};
