// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundObjectInPool.h"
#include "..\Manager\JesusGameInstance.h"
#include "..\Manager\SoundManager.h"

ASoundObjectInPool::ASoundObjectInPool()
{
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
}

void ASoundObjectInPool::PlaySound()
{
	if (IsRandomPitch)
	{
		float value = FMath::RandRange(0.125f, 1.0f);
		AudioComp->SetPitchMultiplier(value);
	}
	
	AudioComp->SetPitchMultiplier(1.0f);
	AudioComp->Play(0.0f);
}

void ASoundObjectInPool::SetActive(bool active)
{
	Super::SetActive(active);
	if (active)
	{
		AudioComp->Activate();
		PlaySound();
		if (LifeTime > 0)
		{
			GetWorldTimerManager().SetTimer(LifeTimeHandler, this, &ASoundObjectInPool::ReturnObject, LifeTime);
		}
	}
}

void ASoundObjectInPool::BeginPlay()
{
	Super::BeginPlay();
	SetActive(false);
	AudioComp->Deactivate();

	SetActorTickEnabled(false);
	if (AudioType == EAudioType::SFX)
	{
		ASoundManager::GetInstance().SFXAudioCompArray.AddUnique(AudioComp);
	}
	else if (AudioType == EAudioType::VOICE)
	{
		ASoundManager::GetInstance().VoiceAudioCompArray.AddUnique(AudioComp);
	}
}

void ASoundObjectInPool::ReturnObject()
{
	Super::ReturnObject();
	//UE_LOG(LogTemp, Warning, TEXT("Return"));
}