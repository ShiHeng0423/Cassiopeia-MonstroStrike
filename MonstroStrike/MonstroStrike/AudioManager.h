#pragma once
#include "AEEngine.h"

enum Audio_List
{
	MAINMENU_SONG,
	ATTACK_SLASH_SFX,
	TOTAL_AUDIO
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

	AEAudio audio[TOTAL_AUDIO];

	AEAudioGroup sfxGroup;
	AEAudioGroup bgmGroup;
};