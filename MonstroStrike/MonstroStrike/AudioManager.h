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
	void SetSFXVolume(f32 vol);
	void SetBGMVolume(f32 vol);

private:
	f32 sfxVolume;
	f32 bgmVolume;

	AEAudio audio[Total_Audio];

	AEAudioGroup sfxGroup;
	AEAudioGroup bgmGroup;
};