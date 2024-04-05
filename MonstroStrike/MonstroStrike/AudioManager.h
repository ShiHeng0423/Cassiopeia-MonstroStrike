/*!************************************************************************
  \file					AudioManager.h
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \brief				File containing declaration of class AudioManager
						that handles all the game audios for the game.

						Audio_List enum defines a list of audio elements 
						used within the game.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#pragma once
#include "AEEngine.h"

enum Audio_List
{
	MAINMENU_SONG,
	ATTACK_SLASH_SFX,
	INVENTORY_OPEN,
	ITEM_EQUIP,
	ITEM_UNEQUIP,
	ITEM_CLICK,
	ITEM_RELEASE,
	ITEM_SWAP,
	REJECT_SFX,
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
