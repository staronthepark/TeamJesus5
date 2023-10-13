// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\EnemyMonster.h"
#include "..\..\Player\PlayerCharacter.h"
#include "..\..\UI\MonsterHPUI.h"
#include "..\..\UI\MonsterWidget.h"
#include "Components/WidgetComponent.h"
#include "Jamsig.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API AJamsig : public AEnemyMonster
{
	GENERATED_BODY()

public:
	AJamsig();
	
};
