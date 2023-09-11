// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../SERVAME/Player/PlayerCharacter.h"
#include "JesusSaveGame.generated.h"

UCLASS()
class SERVAME_API UJesusSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	FString SaveSlotName;	//Save game data file name
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	int32 SaveIndex;	//Save game data file index

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	FVector PlayerLoc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	FRotator PlayerRot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		FPlayerCharacterDataStruct PlayerData;

	
public:
	void SavePlayerData(FPlayerCharacterDataStruct data);
	void SaveLoc(FVector Vec);
	void SaveRot(FRotator Rot);
	void Save(UJesusSaveGame* Instance);
	UJesusSaveGame* Load(UJesusSaveGame* LoadInstance);
};
