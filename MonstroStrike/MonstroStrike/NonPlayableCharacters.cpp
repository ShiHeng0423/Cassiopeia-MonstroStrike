#include "NonPlayableCharacters.h"

//DISCLAIMER: NOTE THAT IT IS ONLY 3 NOW UNLESS SUBJECT TO CHANGES

namespace {
	struct NON_PLAYABLE_CHARACTERS npcs[3];

	void CreateNPCInstance(f32 xPos, f32 yPos, f32 xSize, f32 ySize, NON_PLAYABLE_CHARACTERS& npc, TYPE_NPCS npcType);
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

void InitializeNPC()
{
	CreateNPCInstance(80.f, -155.f, 70.f, 70.f, npcs[0], NPC_BLACKSMITH_A);
	CreateNPCInstance(160.f, -155.f, 70.f, 70.f, npcs[1], NPC_BLACKSMITH_B);
	CreateNPCInstance(240.f, -155.f, 70.f, 70.f, npcs[2], NPC_QUEST_GIVER);
}

void UpdateNPC()
{
	//Insert based on NPC TYPE
	for (s8 i = 0; i < 3; i++)
	{
		switch (npcs[i].typeOfNPC)
		{
		case NPC_BLACKSMITH_A:
			//Here to pop out options / messages
			break;
		case NPC_BLACKSMITH_B:
			//Here to pop out options / messages
			break;		
		case NPC_QUEST_GIVER:
			//Here to pop out options / messages
			break;
		default:
			std::cout << "NPC " << i << " does not have a type?\n";
			break;
		}
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
	void CreateNPCInstance(f32 xPos, f32 yPos, f32 xSize, f32 ySize, NON_PLAYABLE_CHARACTERS& npc, TYPE_NPCS npcType)
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