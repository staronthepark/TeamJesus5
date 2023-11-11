// Fill out your copyright notice in the Description page of Project Settings.

#include "SERVAME/SERVAME.h"
#include "MonsterSoundObjectInpool.h"

AMonsterSoundObjectInpool::AMonsterSoundObjectInpool()
{
}

void AMonsterSoundObjectInpool::BeginPlay()
{
}

void AMonsterSoundObjectInpool::SetActive(bool active)
{
}

void AMonsterSoundObjectInpool::ReturnObject()
{
	Super::ReturnObject();
}

void AMonsterSoundObjectInpool::PlayMonsterSound(EMonsterAudioType Type)
{ 
	if (SoundMap.Contains(Type))
	{
		MonsterAudioType = Type;

		Super::SetActive(true);
		AudioComp->SetSound(SoundMap[Type]);
		AudioComp->SetVolumeMultiplier(Volume);
		PlaySound();

		LOG_S(Warning);
	}
}
