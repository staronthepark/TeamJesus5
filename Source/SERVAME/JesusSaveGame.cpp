// Fill out your copyright notice in the Description page of Project Settings.

#include "JesusSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UJesusSaveGame::SaveLoc(FVector Vec)
{ 
	PlayerLocStruct.PlayerLoc.X = Vec.X;
	PlayerLocStruct.PlayerLoc.Y = Vec.Y;
	PlayerLocStruct.PlayerLoc.Z = Vec.Z;
}

void UJesusSaveGame::SaveRot(FRotator Rot) 
{
	PlayerRotStruct.PlayerRot.Pitch = Rot.Pitch;
	PlayerRotStruct.PlayerRot.Yaw = Rot.Yaw;
	PlayerRotStruct.PlayerRot.Roll = Rot.Roll;
}

void UJesusSaveGame::SaveHealCount(int Count) { HealCount = Count; }

void UJesusSaveGame::SaveSavePoint() {	}

void UJesusSaveGame::Save(UJesusSaveGame* Instance)
{
	Instance->SaveSlotName = "JesusSave";
	Instance->SaveIndex = 0;

	if (UGameplayStatics::SaveGameToSlot(Instance, Instance->SaveSlotName, Instance->SaveIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Succed!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Failed!"));
	}
}

void UJesusSaveGame::Load(UJesusSaveGame* LoadInstance)
{
	//UJesusSaveGame* LoadInstance = Cast<UJesusSaveGame>(UGameplayStatics::CreateSaveGameObject(UJesusSaveGame::StaticClass()));

	LoadInstance->SaveSlotName = "JesusSave";
	LoadInstance->SaveIndex = 0;

	LoadInstance = Cast<UJesusSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadInstance->SaveSlotName, LoadInstance->SaveIndex));

	UE_LOG(LogTemp, Warning, TEXT("SaveName : %s"), *LoadInstance->SaveSlotName);
	UE_LOG(LogTemp, Warning, TEXT("SaveLoc : (%f, %f, %f)"), LoadInstance->PlayerLocStruct.PlayerLoc.X, LoadInstance->PlayerLocStruct.PlayerLoc.Y, LoadInstance->PlayerLocStruct.PlayerLoc.Z);
	UE_LOG(LogTemp, Warning, TEXT("SaveRot : [%f, %f, %f]"), LoadInstance->PlayerRotStruct.PlayerRot.Roll, LoadInstance->PlayerRotStruct.PlayerRot.Pitch, LoadInstance->PlayerRotStruct.PlayerRot.Yaw);
	UE_LOG(LogTemp, Warning, TEXT("SaveHealCount : %d"), LoadInstance->HealCount);
}
