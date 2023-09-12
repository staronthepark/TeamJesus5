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

	SaveInstance->PlayerData = Player->PlayerDataStruct;
	SaveInstance->PlayerLoc = Player->GetActorLocation();
	SaveInstance->PlayerRot = Player->GetActorRotation();

	SaveInstance->SaveSlotName = "JesusSave";
	SaveInstance->SaveIndex = 0;
	SaveInstance->IsPhaseTwo = Player->IsPhaseTwo;

	if (UGameplayStatics::SaveGameToSlot(SaveInstance, SaveInstance->SaveSlotName, SaveInstance->SaveIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("DO"));
	}
}

UJesusSaveGame* UJesusSaveGame::Load(APlayerCharacter* Player)
{
	SaveInstance = Cast<UJesusSaveGame>(UGameplayStatics::CreateSaveGameObject(UJesusSaveGame::StaticClass()));
	SaveInstance->SaveSlotName = "JesusSave";
	SaveInstance->SaveIndex = 0;
	SaveInstance = Cast<UJesusSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveInstance->SaveSlotName, SaveInstance->SaveIndex));
	
	if (SaveInstance != nullptr)
	{
		Player->PlayerDataStruct = SaveInstance->PlayerData;
		Player->SetActorLocation(SaveInstance->PlayerLoc);
		Player->SetActorRotation(SaveInstance->PlayerRot);
		Player->IsPhaseTwo = SaveInstance->IsPhaseTwo;
	}
	
	return SaveInstance;
}
