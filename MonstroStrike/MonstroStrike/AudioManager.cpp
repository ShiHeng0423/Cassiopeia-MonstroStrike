#include "AudioManager.h"

AudioManager::AudioManager()
{
	// Loads a sound from a file named 'bouken.mp3' in the 'Assets' folder
	// and assign it to 'bouken'.
	audio[MAINMENU_SONG] = AEAudioLoadMusic("Assets/Audio/BGM/GameBGM.wav");
	audio[ATTACK_SLASH_SFX] = AEAudioLoadMusic("Assets/Audio/SFX/Attack.wav");
	audio[REJECT_SFX] = AEAudioLoadMusic("Assets/Audio/SFX/Error.wav");

	audio[INVENTORY_OPEN] = AEAudioLoadSound("Assets/Audio/Inventory_SFX/inventory_open.mp3");
	audio[ITEM_EQUIP] = AEAudioLoadSound("Assets/Audio/Inventory_SFX/item_equip.mp3");
	audio[ITEM_CLICK] = AEAudioLoadSound("Assets/Audio/Inventory_SFX/item_click.mp3");
	audio[ITEM_RELEASE] = AEAudioLoadSound("Assets/Audio/Inventory_SFX/item_release.mp3");
	audio[ITEM_UNEQUIP] = AEAudioLoadSound("Assets/Audio/Inventory_SFX/item_unequip.mp3");
	audio[ITEM_SWAP] = AEAudioLoadSound("Assets/Audio/Inventory_SFX/item_swap.mp3");

	bgmGroup = AEAudioCreateGroup();
	sfxGroup = AEAudioCreateGroup();
	this->bgmVolume = this->sfxVolume = 0.5f;
}

AudioManager::~AudioManager()
{
	// Release our audios.
	AEAudioUnloadAudio(audio[MAINMENU_SONG]);
	AEAudioUnloadAudio(audio[ATTACK_SLASH_SFX]);
	AEAudioUnloadAudio(audio[ITEM_EQUIP]);
	AEAudioUnloadAudio(audio[ITEM_CLICK]);
	AEAudioUnloadAudio(audio[ITEM_RELEASE]);
	AEAudioUnloadAudio(audio[ITEM_UNEQUIP]);

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
