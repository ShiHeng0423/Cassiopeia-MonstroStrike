#pragma once
#include "AEEngine.h"

struct Position {
	f32 x;
	f32 y;
};

struct Sprite {
	AEGfxTexture* pTex;
	f32 scale;
};

struct Player {
	Position pos;
	f32 speed;
	Sprite img;
}extern player{ {0.f,0.f},10.f,{nullptr,1000.f} };