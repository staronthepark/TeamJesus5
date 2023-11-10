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
	MonsterAudioType = Type;
	 
	if (SoundMap.Contains(Type))
	{
		Super::SetActive(true);
		AudioComp->SetSound(SoundMap[Type]);
		AudioComp->SetVolumeMultiplier(Volume);
		PlaySound();

		if (LifeTime > 0)
			GetWorldTimerManager().SetTimer(LifeTimeHandler, this, &AMonsterSoundObjectInpool::ReturnObject, LifeTime);

		LOG_S(Warning);
	}
}
