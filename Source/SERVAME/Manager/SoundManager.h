// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "SoundManager.generated.h"



UENUM(BlueprintType)
enum class BGMType : uint8
{
	MAINHALL,
	NUNINTRO,
	NUNLOOP,
	TITLEINTRO,
	TITLEVERSE1,
	TITLEVERSE2,
	TITLEEND,
	CYMBAL,
	DRUM,
};


UCLASS(Blueprintable)
class SERVAME_API ASoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASoundManager();

	UPROPERTY()
		TArray<UAudioComponent*> SFXAudioCompArray;	

	UPROPERTY()
		TArray<UAudioComponent*> VoiceAudioCompArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<BGMType, UAudioComponent*> BGMAudioCompMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAudioComponent* CymbalAudio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAudioComponent* DrumAudio;

	static ASoundManager* Instance;
	static ASoundManager& GetInstance();

	float BGMVolume;
	float SFXVolume;
	float VoiceVolume;

	FTimerHandle DrumTimer;

	BGMType CurrentBGMPlayType;

public:

	bool IsMute;

	void Init();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void MuteSound(bool _IsMute);

	void SetBGMVolume(float value);
	void SetSFXVolume(float value);
	void SetVoiceVolume(float value);

	bool IsMuteSound();
	float GetBGMVolume();
	float GetSFXVolume();
	float GetVoiceVolume();

	void PlayDrumSound();
	void PlaySoundWithCymbalSound(BGMType Type, bool PlayCymbal);
	void PauseBGM();
};
