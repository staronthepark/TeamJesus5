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

void UJesusSaveGame::Save(APlayerCharacter* Player, UJesusGameInstance* GameInstance)
{
	SaveInstance = Cast<UJesusSaveGame>(UGameplayStatics::CreateSaveGameObject(UJesusSaveGame::StaticClass()));

	SaveInstance->PlayerData = Player->PlayerDataStruct;
	SaveInstance->PlayerLoc = Player->GetActorLocation();
	SaveInstance->PlayerRot = Player->GetActorRotation();

	SaveInstance->SaveSlotName = "JesusSave";
	SaveInstance->SaveIndex = 0;
	SaveInstance->IsPhaseTwo = Player->IsPhaseTwo;

	for (int32 i = 0; i < 20; i++)
	{
		if (GameInstance->SavedTriggerActor.Contains(i))
		{
			if (!SaveInstance->SavedTriggerActor.Contains(i))
			{
				SaveInstance->SavedTriggerActor.Add(i, GameInstance->SavedTriggerActor[i]->IsActive);
			}
		}
	}

	if (UGameplayStatics::SaveGameToSlot(SaveInstance, SaveInstance->SaveSlotName, SaveInstance->SaveIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("DO"));
	}
}

UJesusSaveGame* UJesusSaveGame::Load(APlayerCharacter* Player, UJesusGameInstance* GameInstance)
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
		//Player->YawRotation.Yaw = SaveInstance->PlayerRot.Yaw;
		Player->IsPhaseTwo = SaveInstance->IsPhaseTwo;

		for (int32 i = 0; i < 20; i++)
		{
			if (SaveInstance->SavedTriggerActor.Contains(i))
			{
				GameInstance->SavedTriggerActor[i]->IsActive = SaveInstance->SavedTriggerActor[i];
				GameInstance->SavedTriggerActor[i]->Init();
			}
		}
	}

	
	return SaveInstance;
}
