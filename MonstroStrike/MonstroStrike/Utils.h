#pragma once
#include <string>

#include "AEEngine.h"

//#include "Inventory.h"
struct Sprite
{
	AEGfxTexture* pTex;
	AEVec2 scale;
};


struct Sprite_V2
{
	AEGfxTexture* pTex;
	AEVec2 scale;
	AEVec2 pos;
};

struct Object
{
	AEVec2 pos;
	AEVec2 speed;
	Sprite img;
};

struct Button
{
	AEGfxTexture* pTex;
	AEVec2 scale;
	AEVec2 pos;
	void (*Ptr)(void);
};


AEGfxVertexList* GenerateSquareMesh(u32 MeshColor);
AEGfxVertexList* GenerateLineMesh(u32 MeshColor);

//
void ResumeGame();
void ReturnLobby();
void OpenControls();
void QuitMainmenu();
void QuitConfirmation();
void BackPauseMenu();

void Equip(int index, ButtonGearUI tmp);
