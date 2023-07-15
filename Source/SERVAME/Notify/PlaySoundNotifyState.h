// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "..\Boss_1\JesusBoss.h"
#include "PlaySoundNotifyState.generated.h"


UENUM()
enum SoundType
{
	BOSSSWING = 24,
	BOSSPOWERSWING,
	BOSSSOUND27 =27,
	BOSSCRACK,
	BOSSSOUND30 = 30,
};

UCLASS()
class SERVAME_API UPlaySoundNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);

	AJesusBoss* Boss;

public:
	UPlaySoundNotifyState();

	UPROPERTY(EditAnywhere)
	TEnumAsByte<SoundType> Type;
};
