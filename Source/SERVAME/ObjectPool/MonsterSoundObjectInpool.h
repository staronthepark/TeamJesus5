// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoundObjectInPool.h"
#include "MonsterSoundObjectInpool.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMonsterAudioType : uint8
{
	NONE,
	Knight_Attack,
	Knight_Die,
};


UCLASS()
class SERVAME_API AMonsterSoundObjectInpool : public ASoundObjectInPool
{
	GENERATED_BODY()

public:
	AMonsterSoundObjectInpool();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMonsterAudioType MonsterAudioType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EMonsterAudioType, USoundBase*> SoundMap;

	virtual void SetActive(bool active) override;
	virtual void BeginPlay()override;
	virtual void ReturnObject()override;
	void PlayMonsterSound(EMonsterAudioType Type);
};
