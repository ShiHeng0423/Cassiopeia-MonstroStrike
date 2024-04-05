#pragma once
/*!************************************************************************
  \file					NonPlayableCharacters.h
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (85%)
  \secondary author		Teng Shi Heng (10%, helped with collision of NPC),
						Keith Ng Hon Yew (5%, provided function to check for crafting functions)
  \brief				File containing declarations of structures and functions for non-playable characters (NPCs).

  This file contains declarations of structures and functions essential for managing non-playable characters (NPCs), providing 
  services, quests, or other interactions. The header defines structures for representing NPCs, conversation content, and 
  content bars, along with functions for loading, initializing, updating, drawing, and freeing NPC-related resources.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#include <AEEngine.h>
#include "CollisionShape.h"
#include "Player.h"
#include <vector>

enum NpcTypes {
	NPC_CLERIC = 0, //Armor black smith
	NPC_BLACKSMITH_B, //Weapon black smith
	NPC_QUEST_GIVER //Quest give
	//Might add an NPC who restore health
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

	const char* npcName;
	const char* openingText;
	const char* exitingText;
};

struct ConversationContent {
	AEVec2 position;
	AEVec2 size;
	AEVec2 rotate;

	AEMtx33 transformation;
	AEMtx33 translation;
	AEMtx33 rotation;
	AEMtx33 scale;

	AEGfxTexture* conversationBoxSprite;
};

struct ContentBar {
	AEVec2 position;
	AEVec2 size;
	AEVec2 rotate;

	AEMtx33 transformation;
	AEMtx33 translation;
	AEMtx33 rotation;
	AEMtx33 scale;

	AABB collisionBox;
};

//Now just generalize 3 NPCs first...
//NPC 1 -> Cleric, NPC 2 -> Black Smith B, NPC 3 ->Quest giver

void LoadNPC(); //Load textures, now only spawn at lobby level so don't need indicate which level i guess
void InitializeNPC(std::vector<AEVec2> allocatedPositions); //Initialize positions etc
void UpdateNPC(Player* player); //Update
void FreeNPC(); //Free textures
void DrawNPC(AEGfxVertexList& mesh);

//For conversational box
void DrawConvBox(bool inConv, AEGfxVertexList& mesh);