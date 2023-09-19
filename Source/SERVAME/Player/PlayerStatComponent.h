// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Player/PlayerCharacter.h"
#include "PlayerStatComponent.generated.h"


USTRUCT(BlueprintType)
struct FPlayerStatInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SoulCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Value;

	TFunction<bool()> Func;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERVAME_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerStatComponent();

	UPROPERTY(EditAnywhere)
	TArray<FPlayerStatInfo>StrengthStatList;
	UPROPERTY(EditAnywhere)
		TArray<FPlayerStatInfo>StaminaStatList;
	UPROPERTY(EditAnywhere)
		TArray<FPlayerStatInfo>HpStatList;
	UPROPERTY(EditAnywhere)
	TArray<FPlayerStatInfo>ShieldStatList;

private:
	APlayerCharacter* Owner;

protected:
	virtual void BeginPlay() override;
};
