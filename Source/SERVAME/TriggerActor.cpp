// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerActor.h"

ATriggerActor::ATriggerActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATriggerActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATriggerActor::BeginTriggerEvent()
{
	EnableEvent();
}

void ATriggerActor::EndTriggerEvent()
{
	Super::EndTriggerEvent();
}

void ATriggerActor::EnableEvent()
{
}