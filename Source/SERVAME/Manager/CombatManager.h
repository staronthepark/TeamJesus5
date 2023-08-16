// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "..\BaseCharacter.h"
#include "CombatManager.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UCombatManager : public UObject
{
	GENERATED_BODY()

public:
	UCombatManager();
	~UCombatManager();

		static UCombatManager* Instance;

		static UCombatManager& GetInstance();

		UPROPERTY(EditAnywhere)
			TArray<ABaseCharacter*> HitMonsterInfoArray;
		UPROPERTY(EditAnywhere)
			TArray<ABaseCharacter*> MonsterInfoArray;

		void ActivateCollider();
		void DeactivateCollider();

};
