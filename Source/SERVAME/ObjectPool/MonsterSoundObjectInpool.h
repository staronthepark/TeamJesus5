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
	KNIGHT_ATTACK,
	KNIGHT_DIE,
	
	NUN_CRYSTAL_CHARGE,
	NUN_CRYSTAL_BURST,
	NUN_DARKNESS_CHARGE,
	NUN_DARKNESS_BURST,
	NUN_FOG,
	NUN_FRAGMENT_CHARGE,
	NUN_FRAGMENT_BURST,
	NUN_HEAL,
	NUN_JUDEMENT_CHARGE,
	NUN_JUDEMENT_BURST,
	NUN_WORSHIP,
	NUN_HIT1,
	NUN_HIT2,
	NUN_HIT3,
	NUN_DIE,
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
