#include "SoundManager.h"

ASoundManager* ASoundManager::Instance = nullptr;

ASoundManager::ASoundManager()
{
	Instance = this;

	BGMAudioCompArray.Add(CreateDefaultSubobject<UAudioComponent>("Intro_Phase1"));
	BGMAudioCompArray.Add(CreateDefaultSubobject<UAudioComponent>("Verse_Phase1"));
	BGMAudioCompArray.Add(CreateDefaultSubobject<UAudioComponent>("Build_Up_Phase1"));
	BGMAudioCompArray.Add(CreateDefaultSubobject<UAudioComponent>("Verse2_Phase1"));
	BGMAudioCompArray.Add(CreateDefaultSubobject<UAudioComponent>("Outro_Phase1"));

	CymbalAudio = CreateDefaultSubobject<UAudioComponent>("Cymbal");
	DrumAudio = CreateDefaultSubobject<UAudioComponent>("Drum");

}

ASoundManager& ASoundManager::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<ASoundManager>();
	}
	return *Instance;
}

void ASoundManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SFXAudioCompArray.Empty();
	BGMAudioCompArray.Empty();
	VoiceAudioCompArray.Empty();
}

void ASoundManager::Init()
{
	PlayNextBGM = false;
	CurrentIndex = 0;
	MaxTempoTime = 0.4511278195488722f;
	BGMVolume = 1.0f;
	SFXVolume = 1.0f;
	VoiceVolume = 1.0f;


	BGMAudioCompArray.AddUnique(CymbalAudio);
	BGMAudioCompArray.AddUnique(DrumAudio);
	
	BGMMaxIndex = BGMAudioCompArray.Num();
	for (int32 i = 0; i < BGMAudioCompArray.Num(); i++)
	{
		BGMAudioCompArray[i]->SetPaused(true);
	}

	CymbalAudio->SetPaused(true);
	DrumAudio->SetPaused(true);

	SetSFXVolume(SFXVolume);
	SetVoiceVolume(VoiceVolume);
	SetBGMVolume(BGMVolume);
}

void ASoundManager::MuteSound(bool _IsMute)
{
	IsMute = _IsMute;
	if (_IsMute)
	{
		SetBGMVolume(0);
		SetSFXVolume(0);
		SetVoiceVolume(0);
	}
	else
	{
		SetBGMVolume(BGMVolume);
		SetSFXVolume(SFXVolume);
		SetVoiceVolume(VoiceVolume);
	}
}

void ASoundManager::SetBGMVolume(float value)
{
	if(value != 0)
		BGMVolume = value;

	for (int32 i = 0; i < BGMAudioCompArray.Num(); i++)
	{
		BGMAudioCompArray[i]->SetVolumeMultiplier(value);
	}
}

void ASoundManager::SetSFXVolume(float value)
{
	if (value != 0)
		SFXVolume = value;

	for (int32 i = 0; i < SFXAudioCompArray.Num(); i++)
	{
		SFXAudioCompArray[i]->SetVolumeMultiplier(value);
	}
}

void ASoundManager::SetVoiceVolume(float value)
{
	if (value != 0)
		VoiceVolume = value;
	
	for (int32 i = 0; i < VoiceAudioCompArray.Num(); i++)
	{
		VoiceAudioCompArray[i]->SetVolumeMultiplier(value);
	}
}

bool ASoundManager::IsMuteSound()
{
	return IsMute;
}

float ASoundManager::GetBGMVolume()
{
	return BGMVolume;
}

float ASoundManager::GetSFXVolume()
{
	return SFXVolume;
}

float ASoundManager::GetVoiceVolume()
{
	return VoiceVolume;
}

void ASoundManager::StartBGMSound()
{
	for (int32 i = 0; i < BGMAudioCompArray.Num(); i++)
	{
		BGMAudioCompArray[i]->SetPaused(true);
	}
	CurrentIndex = 0;
	BGMAudioCompArray[0]->SetPaused(false);
	BGMAudioCompArray[0]->Play(10.8f);
	GetWorldTimerManager().SetTimer(TempoTimer, this, &ASoundManager::PlayNextBGMSound, MaxTempoTime);
}

void ASoundManager::PlayNextBGMSound()
{
	GetWorldTimerManager().SetTimer(TempoTimer, this, &ASoundManager::PlayNextBGMSound, MaxTempoTime);
	
	if (PlayNextBGM)
	{
		if(CurrentIndex >= 3) 
			BGMAudioCompArray[CurrentIndex]->Play(10.8f);
		else
		BGMAudioCompArray[CurrentIndex]->Play(0.0f);
		BGMAudioCompArray[CurrentIndex]->SetPaused(false);
		PlayNextBGM = false;
	}
}

void ASoundManager::PlayDrumSound()
{
	DrumAudio->Play(0.0f);
	DrumAudio->SetPaused(false);
}

void ASoundManager::PlaySoundWithCymbalSound(int32 Index)
{
	BGMAudioCompArray[CurrentIndex]->SetPaused(true);
	CurrentIndex = Index;
	GetWorldTimerManager().SetTimer(DrumTimer, this, &ASoundManager::PlayDrumSound, 0.1f);
	CymbalAudio->Play(0.5f);
	CymbalAudio->SetPaused(false);
	PlayNextBGM = true;
}