// Copyright Epic Games, Inc. All Rights Reserved.

#include "SERVAMEGameMode.h"
#include "./Player/JesusPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ASERVAMEGameMode::ASERVAMEGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn>
		PlayerPawnBPClass(TEXT("/Game/01_Programming/00_BluePrint/00_BP/00_Player/BP_Player"));

	static ConstructorHelpers::FClassFinder<AJesusPlayerController>
		PlayerControllerBPClass(TEXT("/Game/01_Programming/00_BluePrint/00_BP/03_Manager/BP_JesusPlayerController"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}
