#pragma once
#include "AEEngine.h"

struct Sprite {
	AEGfxTexture* pTex;
	AEVec2 scale;
};

struct Sprite_V2 {
	AEGfxTexture* pTex;
	AEVec2 scale;
	AEVec2 pos;
};

struct Object{
	AEVec2 pos;
	AEVec2 speed;
	Sprite img;
};

struct Button {
	AEGfxTexture* pTex;
	AEVec2 scale;
	AEVec2 pos;
	void (*Ptr)(void);
};

struct ButtonGearUI {
	Sprite img;
	AEVec2 pos;
	bool isWeapon;
};