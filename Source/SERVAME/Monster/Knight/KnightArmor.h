// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KnightArmor.generated.h"

USTRUCT(BlueprintType)
struct FArmorDataStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ArmorHp;
};

UCLASS()
class SERVAME_API AKnightArmor : public AActor
{
	GENERATED_BODY()
	
public:	
	AKnightArmor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FArmorDataStruct ArmorDataStruct;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
