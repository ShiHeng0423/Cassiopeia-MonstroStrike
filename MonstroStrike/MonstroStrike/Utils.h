#pragma once
#include "AEEngine.h"

struct Sprite {
	AEGfxTexture* pTex;
	AEVec2 scale;
};

struct Object {
	AEVec2 pos;
	AEVec2 speed;
	Sprite img;
};

struct Button {
	Sprite img;
	AEVec2 pos;
	void (*Ptr)(void);
};