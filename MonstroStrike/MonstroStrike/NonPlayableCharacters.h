#pragma once
#include <AEEngine.h>
#include "CollisionShape.h"
#include "Player.h"
#include <vector>
enum NpcTypes {
	NPC_NONE = 0,
	NPC_BLACKSMITH_A,
	NPC_BLACKSMITH_B,
	NPC_QUEST_GIVER
};

struct NonPlayableCharacters
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

	NpcTypes typeOfNPC;
};

//Now just generalize 3 NPCs first...
//NPC 1 -> Black Smith, NPC 2 -> Black Smith B, NPC 3 ->Quest giver

void LoadNPC(); //Load textures, now only spawn at lobby level so don't need indicate which level i guess
void InitializeNPC(std::vector<AEVec2> allocatedPositions); //Initialize positions etc
void UpdateNPC(Player* player); //Update
void FreeNPC(); //Free textures
void DrawNPC(AEGfxVertexList& mesh);