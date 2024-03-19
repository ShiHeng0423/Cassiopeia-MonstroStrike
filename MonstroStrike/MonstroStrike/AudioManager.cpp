#include "AudioManager.h"

AudioManager::AudioManager()
{
	// Loads a sound from a file named 'bouken.mp3' in the 'Assets' folder
	// and assign it to 'bouken'.
	audio[MAINMENU_SONG] = AEAudioLoadMusic("Assets/bouken.mp3");
	audio[ATTACK_SLASH_SFX] = AEAudioLoadMusic("Assets/bouken.mp3");

	// Creates an audio group named 'bgm'
	bgmGroup = AEAudioCreateGroup();
	sfxGroup = AEAudioCreateGroup();

	// Plays 'bouken' audio in the "bgm" audio group with
	// 50% volume, 200% pitch, looped infinitely.
	this->bgmVolume = this->sfxVolume = 0.5f;
}

AudioManager::~AudioManager()
{
	// Release our audios.
	AEAudioUnloadAudio(audio[MAINMENU_SONG]);
	AEAudioUnloadAudio(audio[ATTACK_SLASH_SFX]);

	// Release our audio groups.
	AEAudioUnloadAudioGroup(bgmGroup);
	AEAudioUnloadAudioGroup(sfxGroup);
}

void AudioManager::PlayAudio(bool isBGM, Audio_List audioName)
{
	if (isBGM)
	{
		AEAudioPlay(audio[audioName], bgmGroup, bgmVolume, 1.f, -1);
	}
	else
	{
		AEAudioPlay(audio[audioName], sfxGroup, sfxVolume, 1.f, 0);
	}
}

void AudioManager::IncreaseSFXVolume()
{
	sfxVolume += 0.05f;
	sfxVolume = AEClamp(sfxVolume, 0.f, 0.5f);
	AEAudioSetGroupVolume(sfxGroup, sfxVolume);
}

void AudioManager::DecreaseSFXVolume()
{
	sfxVolume -= 0.05f;
	sfxVolume = AEClamp(sfxVolume, 0.f, 0.5f);
	AEAudioSetGroupVolume(sfxGroup, sfxVolume);
}

void AudioManager::IncreaseBGMVolume()
{
	bgmVolume += 0.05f;
	bgmVolume = AEClamp(bgmVolume, 0.f, 0.5f);
	AEAudioSetGroupVolume(bgmGroup, bgmVolume);
}

void AudioManager::DecreaseBGMVolume()
{
	bgmVolume -= 0.05f;
	bgmVolume = AEClamp(bgmVolume, 0.f, 0.5f);
	AEAudioSetGroupVolume(bgmGroup, bgmVolume);
}

f32 AudioManager::GetSFXVolume()
{
	return sfxVolume;
}

f32 AudioManager::GetBGMVolume()
{
	return bgmVolume;
}