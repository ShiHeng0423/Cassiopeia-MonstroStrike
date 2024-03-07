#pragma once
#include <string>

#include "AEEngine.h"
//#include "Inventory.h"

namespace Inventory
{
	struct Item
	{
		std::string UID;
		int ID;
		std::string name;
		std::string description;
		int item_type;
		int rarity;
		int quantity;
		bool stackable;
		int attack;
		int defence;
	};
}


struct Sprite
{
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

struct ButtonGearUI
{
	Sprite img;
	AEVec2 pos;
	bool isWeapon;
	Inventory::Item Item;
};


AEGfxVertexList* GenerateSquareMesh(u32 MeshColor);
AEGfxVertexList* GenerateLineMesh(u32 MeshColor);