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