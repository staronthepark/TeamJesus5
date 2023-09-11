// Fill out your copyright notice in the Description page of Project Settings.

#include "JesusSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UJesusSaveGame::SavePlayerData(FPlayerCharacterDataStruct data)
{
	PlayerData = data;
}

void UJesusSaveGame::SaveLoc(FVector Vec)
{ 
	PlayerLoc = Vec;
}

void UJesusSaveGame::SaveRot(FRotator Rot) 
{
	PlayerRot = Rot;
}

void UJesusSaveGame::Save(UJesusSaveGame* Instance)
{
	Instance->SaveSlotName = "JesusSave";
	Instance->SaveIndex = 0;

	UGameplayStatics::SaveGameToSlot(Instance, Instance->SaveSlotName, Instance->SaveIndex);
}

UJesusSaveGame* UJesusSaveGame::Load(UJesusSaveGame* LoadInstance)
{
	LoadInstance->SaveSlotName = "JesusSave";
	LoadInstance->SaveIndex = 0;

	LoadInstance = Cast<UJesusSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadInstance->SaveSlotName, LoadInstance->SaveIndex));

	return LoadInstance;
}
