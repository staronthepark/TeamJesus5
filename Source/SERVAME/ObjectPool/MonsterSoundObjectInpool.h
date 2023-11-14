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
	NUN_KNIGHTSPAWN,
	NUN_SCREAM,

	JAMSIG_IDLE = 49,
	JAMSIG_IDLE_SIT,
	JAMSIG_WALK,
	JAMSIG_WALK2,
	JAMSIG_ATTACK,
	JAMSIG_DASHATTACK,
	JAMSIG_HIT,
	JAMSIG_HIT2,
	JAMSIG_DIE = 57,

	KNIGHT_WALK = 58,
	KNIGHT_WALK2,
	KNIGHT_RUN,
	KNIGHT_RUN2,
	KNIGHT_SLASH1,
	KNIGHT_SLASH2,
	KNIGHT_HIT,
	KNIGHT_HIT2 = 65,

	BOSS_SWING,
	BOSS_POWERSWING,
	BOSS_SOUND27,
	BOSS_CRACK,
	BOSS_SOUND30,

	BOSS_WALK = 71,
	BOSS_WALK2,
	BOSS_WALK3,
	BOSS_WALK4,
	BOSS_WALK5,
	BOSS_WALK6 = 76,

	BOSS_RUN = 77,
	
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
	BOSS2_THROW,
	BOSS2_THROW2,
	BOSS2_CREATECROSS,
	BOSS2_CROSSFALL,
	BOSS2_CROSS_EXPLOSION,
	BOSS2_HIT,
	BOSS2_GROGGY,
	BOSS2_DIE,

	BOSS2_WALK,
	BOSS2_RUN,

	KNIGHT_STINGATTACK,
	KNIGHT_SPRINT_ATTACK
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
