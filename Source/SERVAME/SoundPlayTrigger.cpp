// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundPlayTrigger.h"

ASoundPlayTrigger::ASoundPlayTrigger()
{
	PrimaryActorTick.bCanEverTick = false;
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComp->SetPaused(true);
}

void ASoundPlayTrigger::PlayAudio()
{
	AudioComp->SetPaused(false);
	AudioComp->Play(0.0f);

}

void ASoundPlayTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void ASoundPlayTrigger::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();
}

void ASoundPlayTrigger::EndTriggerEvent()
{
	Super::EndTriggerEvent();
	AudioComp->SetPaused(true);
}

void ASoundPlayTrigger::EnableEvent()
{
	Super::EnableEvent();
	PlayAudio();
}