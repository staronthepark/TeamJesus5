// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "BossBaseNotifyState.h"
#include "..\ObjectPool\MonsterSoundObjectInpool.h"
#include "PlaySoundNotifyState.generated.h"


UCLASS()
class SERVAME_API UPlaySoundNotifyState : public UBossBaseNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);

public:
	UPlaySoundNotifyState();

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMonsterAudioType> Type;
	UPROPERTY(EditAnywhere)
	bool IsRandom = false;
	UPROPERTY(EditAnywhere)
	int Range_Start = 0;
	UPROPERTY(EditAnywhere)
	int Range_End = 0;
};
