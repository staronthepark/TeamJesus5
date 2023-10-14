// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "ChangeLightSettingTriggerActor.h"

AChangeLightSettingTriggerActor::AChangeLightSettingTriggerActor()
{

}

void AChangeLightSettingTriggerActor::BeginPlay()
{
	Super::BeginPlay();

	LightManager = Cast<ALevelLightingManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelLightingManager::StaticClass()));
}

void AChangeLightSettingTriggerActor::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();

}

void AChangeLightSettingTriggerActor::EndTriggerEvent()
{
	Super::EndTriggerEvent();

}

void AChangeLightSettingTriggerActor::EnableEvent()
{
	Super::EnableEvent();
	LightManager->ChangeTargetLightSetting(MapName);
}
