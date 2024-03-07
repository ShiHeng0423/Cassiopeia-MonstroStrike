#pragma once
#include "AEEngine.h"

enum Audio_List
{
	MainMenu,
	Attack_Slash,
	Total_Audio
};

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	void PlayAudio(bool isBGM, Audio_List audioName);
	void SetSFXVolume();
	void SetBGMVolume();

private:
	f32 sfxVolume;
	f32 bgmVolume;

	AEAudio audio[Total_Audio];

	AEAudioGroup sfxGroup;
	AEAudioGroup bgmGroup;
};