#include "NonPlayableCharacters.h"
#include "Player.h"
#include <vector>
#include <algorithm>

//DISCLAIMER: NOTE THAT IT IS ONLY 3 NOW UNLESS SUBJECT TO CHANGES

namespace {
	struct NonPlayableCharacters npcs[3];

	void CreateNPCInstance(f32 xPos, f32 yPos, f32 xSize, f32 ySize, NonPlayableCharacters& npc, NpcTypes npcType);
}



void LoadNPC()
{
	npcs[0].pTexPortrait = AEGfxTextureLoad("Assets/NPCs/NPC_Blacksmith_A_Portrait.png");
	npcs[1].pTexPortrait = AEGfxTextureLoad("Assets/NPCs/NPC_Blacksmith_B_Portrait.png");
	npcs[2].pTexPortrait = AEGfxTextureLoad("Assets/NPCs/NPC_Blacksmith_A_Portrait.png");

	npcs[0].pTexSprite = AEGfxTextureLoad("Assets/SubaDuck.png");
	npcs[1].pTexSprite = AEGfxTextureLoad("Assets/SubaDuck.png");
	npcs[2].pTexSprite = AEGfxTextureLoad("Assets/SubaDuck.png");
}

void InitializeNPC(std::vector<AEVec2> allocatedPositions)
{
	for (int i = 0; i < allocatedPositions.size(); i++)
	{
		AEVec2 pos = allocatedPositions[i];
		switch (i) {
		case 0:
			CreateNPCInstance(pos.x, pos.y * 0.96f, 70.f, 70.f, npcs[i], NPC_BLACKSMITH_A);
			break;
		case 1:
			CreateNPCInstance(pos.x, pos.y * 0.96f, 70.f, 70.f, npcs[i], NPC_BLACKSMITH_B);
			break;
		case 2:
			CreateNPCInstance(pos.x, pos.y * 0.96f, 70.f, 70.f, npcs[i], NPC_QUEST_GIVER);
			break;
		default:
			// Handle error or ignore if necessary
			std::cerr << "Went out of range for enemy\n";
			break;
		}
	}
}

void UpdateNPC(Player* player)
{
	std::vector<std::pair<f32, s8>>collidedPlayer;
	//Insert based on NPC TYPE
	for (s8 i = 0; i < 3; i++)
	{
		switch (npcs[i].typeOfNPC)
		{
		case NPC_BLACKSMITH_A:
			//Here to pop out options / messages to be able to interact to this npc
			if (AABBvsAABB(player->collisionBox, npcs[i].collisionBox))
			{
				collidedPlayer.push_back({ AEVec2Distance(&player->obj.pos, &npcs[i].position), i });
			}
			break;
		case NPC_BLACKSMITH_B:
			//Here to pop out options / messages
			if (AABBvsAABB(player->collisionBox, npcs[i].collisionBox))
			{
				collidedPlayer.push_back({ AEVec2Distance(&player->obj.pos, &npcs[i].position), i });
			}
			break;		
		case NPC_QUEST_GIVER:
			//Here to pop out options / messages
			if (AABBvsAABB(player->collisionBox, npcs[i].collisionBox))
			{
				collidedPlayer.push_back({ AEVec2Distance(&player->obj.pos, &npcs[i].position), i });
			}
			break;
		default:
			std::cout << "NPC " << i << " does not have a type?\n";
			break;
		}
	}

	std::sort(collidedPlayer.begin(), collidedPlayer.end());
	if (AEInputCheckTriggered(AEVK_I))
	{
		//interact
		//function call (collidedPlayer[0].second) -> nearest npc to player
	}
}

void DrawNPC(AEGfxVertexList& mesh)
{
	for (s8 i = 0; i < 3; i++)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(npcs[i].pTexPortrait, 0, 0);
		AEGfxSetTransform(npcs[i].transformation.m);
		AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);
	}
}
void FreeNPC()
{
	for (s8 i = 0; i < 3; i++)
	{
		AEGfxTextureUnload(npcs[i].pTexPortrait);
		AEGfxTextureUnload(npcs[i].pTexSprite);
	}
}
namespace {
	void CreateNPCInstance(f32 xPos, f32 yPos, f32 xSize, f32 ySize, NonPlayableCharacters& npc, NpcTypes npcType)
	{
		npc.rotation = { 0 };
		AEMtx33Rot(&npc.rotation, 0.f);

		AEVec2Set(&npc.size, xSize, ySize);
		AEMtx33Scale(&npc.scale, npc.size.x, npc.size.y);

		AEVec2Set(&npc.position, xPos, yPos);
		AEMtx33Trans(&npc.translation, npc.position.x, npc.position.y);

		npc.transformation = { 0 };
		AEMtx33Concat(&npc.transformation, &npc.rotation, &npc.scale);
		AEMtx33Concat(&npc.transformation, &npc.translation, &npc.transformation);

		npc.typeOfNPC = npcType;

		//AABB Box
		npc.collisionBox.minimum.x = npc.position.x - npc.size.x * 0.5f;
		npc.collisionBox.minimum.y = npc.position.y - npc.size.y * 0.5f;
		npc.collisionBox.maximum.x = npc.position.x + npc.size.x * 0.5f;
		npc.collisionBox.maximum.y = npc.position.y + npc.size.y * 0.5f;

		//Does not need put in update considering NPCS are at static position
	}
}