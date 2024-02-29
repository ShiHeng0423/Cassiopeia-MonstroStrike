#pragma once
#include <AEEngine.h>
#include "CollisionShape.h"

enum TYPE_NPCS {
	NPC_NONE = 0,
	NPC_BLACKSMITH_A,
	NPC_BLACKSMITH_B,
	NPC_QUEST_GIVER
};

struct NON_PLAYABLE_CHARACTERS
{
	AEVec2 position;
	AEVec2 size;

	AABB collisionBox;

	AEMtx33 transformation;
	AEMtx33 scale;
	AEMtx33 rotation;
	AEMtx33 translation;

	AEGfxTexture* pTexPortrait;
	AEGfxTexture* pTexSprite;

	TYPE_NPCS typeOfNPC;
};

//Now just generalize 3 NPCs first...
//NPC 1 -> Black Smith, NPC 2 -> Black Smith B, NPC 3 ->Quest giver

void LoadNPC(); //Load textures, now only spawn at lobby level so don't need indicate which level i guess
void InitializeNPC(); //Initialize positions etc
void UpdateNPC(); //Update
void FreeNPC(); //Free textures
void DrawNPC(AEGfxVertexList& mesh);
void UnloadNPC(); //Unload the mesh if have