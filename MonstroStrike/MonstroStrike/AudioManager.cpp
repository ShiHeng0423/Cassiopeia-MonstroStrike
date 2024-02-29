#include "AudioManager.h"

AudioManager::AudioManager()
{
	// Loads a sound from a file named 'bouken.mp3' in the 'Assets' folder
	// and assign it to 'bouken'.
	audio[MainMenu] = AEAudioLoadMusic("Assets/bouken.mp3");
	audio[Attack_Slash] = AEAudioLoadMusic("Assets/bouken.mp3");

	// Creates an audio group named 'bgm'
	bgmGroup = AEAudioCreateGroup();
	sfxGroup = AEAudioCreateGroup();

	// Plays 'bouken' audio in the "bgm" audio group with
	// 50% volume, 200% pitch, looped infinitely.

}
AudioManager::~AudioManager()
{
	// Release our audios.
	AEAudioUnloadAudio(audio[MainMenu]);
	AEAudioUnloadAudio(audio[Attack_Slash]);

	// Release our audio groups.
	AEAudioUnloadAudioGroup(bgmGroup);
	AEAudioUnloadAudioGroup(sfxGroup);
}

void AudioManager::PlayAudio(bool isBGM, Audio_List audioName)
{
	if (isBGM)
	{
		AEAudioPlay(audio[audioName], bgmGroup, 1.f, 1.f, -1);
	}
	else
	{
		AEAudioPlay(audio[audioName], sfxGroup, 1.f, 1.f, 0);

	}
}

void AudioManager::SetSFXVolume()
{

}
void AudioManager::SetBGMVolume()
{

}