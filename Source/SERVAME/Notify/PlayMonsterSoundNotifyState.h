// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBaseNoifyState.h"
#include "..\ObjectPool\MonsterSoundObjectInpool.h"
#include "PlayMonsterSoundNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UPlayMonsterSoundNotifyState : public UMonsterBaseNoifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);

public:
	UPlayMonsterSoundNotifyState();

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMonsterAudioType> Type;

	UPROPERTY(EditAnywhere)
	bool IsRandom;
	UPROPERTY(EditAnywhere)
	int Range_Start;
	UPROPERTY(EditAnywhere)
	int Range_End;
};
