// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "KnightArmor.h"
#include "KnightArmorCollider.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UKnightArmorCollider : public UBoxComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AKnightArmor* Armor;
};
