// Fill out your copyright notice in the Description page of Project Settings.


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
		AudioComp->SetSound(SoundMap[Type]);
		PlaySound();
	}
}
