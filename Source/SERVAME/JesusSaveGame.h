// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "JesusSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FPlayerLocStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	FVector PlayerLoc;
};

USTRUCT(BlueprintType)
struct FPlayerRotStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	FRotator PlayerRot;
};


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
	FPlayerLocStruct PlayerLocStruct;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	FPlayerRotStruct PlayerRotStruct;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	int HealCount;
	
public:
	void SaveLoc(FVector Vec);
	void SaveRot(FRotator Rot);
	void SaveHealCount(int Count);
	void SaveSavePoint();
	void Save(UJesusSaveGame* Instance);
	void Load(UJesusSaveGame* LoadInstance);
};
