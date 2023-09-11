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

void UJesusSaveGame::Save()
{
	UJesusSaveGame* SaveInstance = Cast<UJesusSaveGame>(UGameplayStatics::CreateSaveGameObject(UJesusSaveGame::StaticClass()));

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetOwner());
	PlayerData = Player->PlayerDataStruct;
	PlayerLoc = Player->GetActorLocation();
	PlayerRot = Player->GetActorRotation();

	SaveInstance->SaveSlotName = "JesusSave";
	SaveInstance->SaveIndex = 0;

	UGameplayStatics::SaveGameToSlot(SaveInstance, SaveInstance->SaveSlotName, SaveInstance->SaveIndex);
}

UJesusSaveGame* UJesusSaveGame::Load()
{
	UJesusSaveGame* LoadInstance = Cast<UJesusSaveGame>(UGameplayStatics::CreateSaveGameObject(UJesusSaveGame::StaticClass()));

	LoadInstance->SaveSlotName = "JesusSave";
	LoadInstance->SaveIndex = 0;

	LoadInstance = Cast<UJesusSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("JesusSave"), 0));

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetOwner());
	Player->PlayerDataStruct   = LoadInstance->PlayerData;
	Player->GetActorLocation() = LoadInstance->PlayerLoc ;
	Player->GetActorRotation() = LoadInstance->PlayerRot ;

	return LoadInstance;
}
