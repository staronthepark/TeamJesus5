#include "SoundManager.h"

ASoundManager* ASoundManager::Instance = nullptr;

ASoundManager::ASoundManager()
{
	Instance = this;

	BGMAudioCompMap.Add(BGMType::TITLEINTRO, CreateDefaultSubobject<UAudioComponent>("Intro_Phase1"));
	BGMAudioCompMap.Add(BGMType::TITLEVERSE1, CreateDefaultSubobject<UAudioComponent>("Verse_Phase1"));
	BGMAudioCompMap.Add(BGMType::TITLEVERSE2, CreateDefaultSubobject<UAudioComponent>("Verse2_Phase1"));
	BGMAudioCompMap.Add(BGMType::TITLEEND, CreateDefaultSubobject<UAudioComponent>("Outro_Phase1"));

	BGMAudioCompMap.Add(BGMType::NUNINTRO, CreateDefaultSubobject<UAudioComponent>("Nun_Intro"));
	BGMAudioCompMap.Add(BGMType::NUNLOOP, CreateDefaultSubobject<UAudioComponent>("Nun_Loop"));


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
	BGMAudioCompMap.Empty();
	VoiceAudioCompArray.Empty();
}


void ASoundManager::Init()
{
	BGMVolume = 0.5f;
	SFXVolume = 0.5f;
	VoiceVolume = 0.5f;

	CurrentBGMPlayType = BGMType::TITLEINTRO;

	if(!BGMAudioCompMap.Contains(BGMType::CYMBAL))
	BGMAudioCompMap.Add(BGMType::CYMBAL, CymbalAudio);
	if (!BGMAudioCompMap.Contains(BGMType::DRUM))
		BGMAudioCompMap.Add(BGMType::DRUM, DrumAudio);
	
	for (auto var : BGMAudioCompMap)
	{
		var.Value->SetPaused(true);
		var.Value->SetUISound(true);
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

	for (auto var : BGMAudioCompMap)
	{
		var.Value->SetVolumeMultiplier(value);
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

void ASoundManager::PlayDrumSound()
{
	DrumAudio->Play(0.0f);
	DrumAudio->SetPaused(false);
}

void ASoundManager::PlaySoundWithCymbalSound(BGMType Type, bool PlayCymbal)
{
	BGMAudioCompMap[CurrentBGMPlayType]->SetPaused(true);
	CurrentBGMPlayType = Type;
	BGMAudioCompMap[CurrentBGMPlayType]->SetPaused(false);

	float Time = 10.8f;

	if (Type != BGMType::TITLEINTRO && Type != BGMType::TITLEEND) Time = 0.0f;
	BGMAudioCompMap[CurrentBGMPlayType]->Play(Time);

	if (PlayCymbal)
	{
		GetWorldTimerManager().SetTimer(DrumTimer, this, &ASoundManager::PlayDrumSound, 0.1f);
		CymbalAudio->Play(0.5f);
		CymbalAudio->SetPaused(false);
	}
}

void ASoundManager::PauseBGM()
{
	for (auto var : BGMAudioCompMap)
	{
		var.Value->SetPaused(true);
	}


	DrumAudio->Play(0.0f);
}