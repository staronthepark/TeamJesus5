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

	BOSS_SWING,
	BOSS_POWERSWING,
	BOSS_SOUND27,
	BOSS_CRACK,
	BOSS_SOUND30,
	BOSS_WALK,
	BOSS_RUN,
	BOSS_DASH,
	BOSS_DARKEXPLOSION,
	BOSS_JUMP_UP,
	BOSS_LAND,
	BOSS_SLASH_UP,
	BOSS_SLASH,
	
	BOSS2_JUMP_UP,
	BOSS2_LAND,
	BOSS2_STARTVOMIT,
	BOSS2_VOMIT,
	BOSS2_VOMIT_FALLIN,
	BOSS2_ROAR,
	BOSS2_ROCKHIT,
	BOSS2_RUN,
	BOSS2_THROW,
	BOSS2_THROW2,
	BOSS2_CREATECROSS,
	BOSS2_CROSSFALL,
	BOSS2_CROSS_EXPLOSION,
	BOSS2_HIT,
	BOSS2_GROGGY,
	BOSS2_DIE,

	NUN_KNIGHTSPAWN,
	NUN_SCREAM,

	JAMSIG_IDLE,
	JAMSIG_IDLE_SIT,
	JAMSIG_WALK,
	JAMSIG_WALK2,
	JAMSIG_ATTACK,
	JAMSIG_DASHATTACK,
	JAMSIG_HIT,
	JAMSIG_HIT2,
	JAMSIG_DIE,
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Volume = 1.f;

	virtual void SetActive(bool active) override;
	virtual void BeginPlay()override;
	virtual void ReturnObject()override;
	void PlayMonsterSound(EMonsterAudioType Type);
};
