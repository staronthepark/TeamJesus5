// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../SERVAME/Player/PlayerCharacter.h"
#include "../SERVAME/BaseTriggerActor.h"
#include "JesusSaveGame.generated.h"

UCLASS()
class SERVAME_API UJesusSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UJesusSaveGame();

	/*
	*  ¿µÈ¥ °¹¼ö, È­ÅêºÒ, Æ®¸®°Å ÄÆ¾À ½ÇÇà, Á×Àº º¸½º, ¹® °³¹æ ¿©ºÎ, °ÔÀÓ ¼³Á¤, ÂïÀº ½ºÅÝ,¾ÆÀÌÅÛ, 
	*/

	static UJesusSaveGame* Instance;
	static UJesusSaveGame& GetInstance();

	TArray<ABaseTriggerActor> TriggerActor;

	UJesusSaveGame* SaveInstance;

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


	void Save(APlayerCharacter* Player);

	UJesusSaveGame* Load(APlayerCharacter* Player);
};
