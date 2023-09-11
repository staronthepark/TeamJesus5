// Fill out your copyright notice in the Description page of Project Settings.

#include "JesusSaveGame.h"
#include "Kismet/GameplayStatics.h"

UJesusSaveGame* UJesusSaveGame::Instance = nullptr;
UJesusSaveGame::UJesusSaveGame()
{
	Instance = this;
}

UJesusSaveGame& UJesusSaveGame::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<UJesusSaveGame>();
	}
	return *Instance;
}

void UJesusSaveGame::Save(APlayerCharacter* Player)
{
	SaveInstance = Cast<UJesusSaveGame>(UGameplayStatics::CreateSaveGameObject(UJesusSaveGame::StaticClass()));

	PlayerData = Player->PlayerDataStruct;
	PlayerLoc = Player->GetActorLocation();
	PlayerRot = Player->GetActorRotation();

	SaveInstance->SaveSlotName = "JesusSave";
	SaveInstance->SaveIndex = 0;

	UGameplayStatics::SaveGameToSlot(SaveInstance, SaveInstance->SaveSlotName, SaveInstance->SaveIndex);
}

UJesusSaveGame* UJesusSaveGame::Load(APlayerCharacter* Player)
{
	SaveInstance = Cast<UJesusSaveGame>(UGameplayStatics::CreateSaveGameObject(UJesusSaveGame::StaticClass()));
	SaveInstance->SaveSlotName = "JesusSave";
	SaveInstance->SaveIndex = 0;
	SaveInstance = Cast<UJesusSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveInstance->SaveSlotName, SaveInstance->SaveIndex));

	Player->PlayerDataStruct   = SaveInstance->PlayerData;
	Player->GetActorLocation() = SaveInstance->PlayerLoc ;
	Player->GetActorRotation() = SaveInstance->PlayerRot ;

	return SaveInstance;
}
