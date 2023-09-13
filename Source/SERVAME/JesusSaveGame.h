// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../SERVAME/Player/PlayerCharacter.h"
#include "../SERVAME/BaseTriggerActor.h"
#include "../SERVAME/Manager/JesusGameInstance.h"
#include "JesusSaveGame.generated.h"

UCLASS()
class SERVAME_API UJesusSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UJesusSaveGame();

	/*
	*  ��ȥ ����, ȭ���, Ʈ���� �ƾ� ����, ���� ����, �� ���� ����, ���� ����,������
	*/

	static UJesusSaveGame* Instance;
	static UJesusSaveGame& GetInstance();

	TArray<ABaseTriggerActor> TriggerActor;

	UJesusSaveGame* SaveInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		FName LevelToLoad;

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
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		bool IsPhaseTwo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		TMap<int32, bool>SavedTriggerActor;

public:


	void Save(APlayerCharacter* Player, UJesusGameInstance* Instance);

	UJesusSaveGame* Load(APlayerCharacter* Player, UJesusGameInstance* Instance);
};
