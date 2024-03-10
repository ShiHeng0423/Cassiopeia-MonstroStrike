#pragma once
#include "AEEngine.h"

enum Audio_List
{
	MainMenu_Song,
	Attack_Slash_SFX,
	Total_Audio
};

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	void PlayAudio(bool isBGM, Audio_List audioName);

	f32 GetSFXVolume();
	f32 GetBGMVolume();

	void IncreaseSFXVolume();
	void DecreaseSFXVolume();
	void IncreaseBGMVolume();
	void DecreaseBGMVolume();

private:
	f32 sfxVolume;
	f32 bgmVolume;

	AEAudio audio[Total_Audio];

	AEAudioGroup sfxGroup;
	AEAudioGroup bgmGroup;
};