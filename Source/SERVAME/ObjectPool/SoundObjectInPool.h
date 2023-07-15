// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectInPool.h"
#include "Components/AudioComponent.h"
#include "SoundObjectInPool.generated.h"


UENUM(BlueprintType)
enum class EAudioType : uint8
{
	NONE,
	SFX,
	VOICE,
};

/**
 * 
 */
UCLASS()
class SERVAME_API ASoundObjectInPool : public ABaseObjectInPool
{
	GENERATED_BODY()
	
public:
	ASoundObjectInPool();


	FTimerHandle LifeTimeHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAudioType AudioType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAudioComponent* AudioComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsRandomPitch;

	void PlaySound();
	
	virtual void SetActive(bool active) override;
	virtual void BeginPlay()override;
	virtual void ReturnObject()override;
};
