#include "NonPlayableCharacters.h"
#include "Player.h"
#include <vector>
#include <algorithm>
#include "LevelHeaders.h"

//DISCLAIMER: NOTE THAT IT IS ONLY 3 NOW UNLESS SUBJECT TO CHANGES

namespace {
	struct NonPlayableCharacters npcs[3];

	void CreateNPCInstance(f32 xPos, f32 yPos, f32 xSize, f32 ySize, NonPlayableCharacters& npc, NpcTypes npcType);

	struct conversationBox convBox;
	
	void CreateConvBoxInstance(f32 xPos, f32 yPos, f32 xSize, f32 ySize,
		conversationBox& convBox);
	
	enum ConversationState {
		
		CONVERSATION_OUTSIDE = 0,
		CONVERSATION_ENTRY,
		CONVERSATION_CONTENT,
		CONVERSATION_EXIT
	} currentConvState;

	AEVec2 screenPos;

	std::vector<std::pair<f32, s16>>collidedPlayer;
}

void LoadNPC()
{
	npcs[0].pTexPortrait = AEGfxTextureLoad("Assets/NPCs/NPC_Blacksmith_A_Portrait.png");
	npcs[1].pTexPortrait = AEGfxTextureLoad("Assets/NPCs/NPC_Blacksmith_B_Portrait.png");
	npcs[2].pTexPortrait = AEGfxTextureLoad("Assets/border.png");

	npcs[0].pTexSprite = AEGfxTextureLoad("Assets/NPCs/NPC_Blacksmith_A_Portrait.png");
	npcs[1].pTexSprite = AEGfxTextureLoad("Assets/NPCs/NPC_Blacksmith_B_Portrait.png");
	npcs[2].pTexSprite = AEGfxTextureLoad("Assets/SubaDuck.png");

	convBox.conversationBoxSprite = AEGfxTextureLoad("Assets/ConversationBox.png");

	npcs[0].npcName = "Tom, Armor Blacksmith";
	npcs[1].npcName = "Jack, Weapon Blacksmith";
	npcs[2].npcName = "Marie, Quest Counter";

	npcs[0].openingText = "Heya chump, looking for some armors? [Y/N]";
	npcs[1].openingText = "It is too dangerous to go alone, craft some weapons from me.[Y/N]";
	npcs[2].openingText = "Good day, are there any quests that caught your eye? [Y/N]";

	npcs[0].exitingText = "Storm through, brave soldier. [Y] to continue";
	npcs[1].exitingText = "Feel free to come back anytime. [Y] to continue";
	npcs[2].exitingText = "Looking forward to your next visit! [Y] to continue";
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
	
	AEGfxGetCamPosition(&screenPos.x, &screenPos.y);
	CreateConvBoxInstance(screenPos.x, screenPos.y - (f32)AEGfxGetWindowHeight() * 0.3725f,
		(f32)AEGfxGetWindowWidth(), (f32)AEGfxGetWindowHeight() * 0.25f, convBox);

	currentConvState = CONVERSATION_OUTSIDE; //Not in conversation
}

void UpdateNPC(Player* player)
{
	collidedPlayer.clear(); //Clear vector

	//By Shi heng
	for (s16 i = 0; i < 3; i++) //Check collided NPC
	{
		switch (npcs[i].typeOfNPC)
		{
		case NPC_BLACKSMITH_A:
			if (AABBvsAABB(player->collisionBox, npcs[i].collisionBox))
			{
				collidedPlayer.push_back({ AEVec2Distance(&player->obj.pos, &npcs[i].position), i});
			}
			break;
		case NPC_BLACKSMITH_B:
			if (AABBvsAABB(player->collisionBox, npcs[i].collisionBox))
			{
				collidedPlayer.push_back({ AEVec2Distance(&player->obj.pos, &npcs[i].position), i});
			}
			break;
		case NPC_QUEST_GIVER:
			if (AABBvsAABB(player->collisionBox, npcs[i].collisionBox))
			{
				collidedPlayer.push_back({ AEVec2Distance(&player->obj.pos, &npcs[i].position), i});
			}
			break;
		default:
			std::cout << "NPC " << i << " does not have a type?\n";
			break;
		}
	}

	//Check if there are any collided NPC
	if (AEInputCheckTriggered(AEVK_F) && !collidedPlayer.empty())
	{
		std::sort(collidedPlayer.begin(), collidedPlayer.end()); //Sort the closest collided NPC to be front
		currentConvState = CONVERSATION_ENTRY;
		player->isConversation = true;
	}

	if (player->isConversation)
	{
		switch (currentConvState)
		{
		case CONVERSATION_ENTRY:
			if (AEInputCheckTriggered(AEVK_Y))
			{
				//Activate next conversation
				//currentConvState = CONVERSATION_CONTENT;
			}
			else if (AEInputCheckTriggered(AEVK_N))
			{
				currentConvState = CONVERSATION_EXIT;
			}
			break;
		case CONVERSATION_CONTENT:
			//Check which content to display
			switch (collidedPlayer[0].second)
			{
			case NPC_BLACKSMITH_A:
				break;
			case NPC_BLACKSMITH_B:
				break;
			case NPC_QUEST_GIVER:
				break;
			}
			break;
		case CONVERSATION_EXIT:
			if (AEInputCheckTriggered(AEVK_Y))
			{
				currentConvState = CONVERSATION_OUTSIDE;
			}
			break;
		case CONVERSATION_OUTSIDE:
		default:
			player->isConversation = false;
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

void DrawConvBox(bool inConv, AEGfxVertexList& mesh)
{
	AEGfxGetCamPosition(&screenPos.x, &screenPos.y);
	AEMtx33 transformation, scale, rotation, translation;

	switch (currentConvState)
	{
	case CONVERSATION_ENTRY:
		//NPC portrait (Should be at left)
		AEMtx33Rot(&rotation, 0.f);
		AEMtx33Scale(&scale, AEGfxGetWindowWidth() * 0.25f, AEGfxGetWindowHeight() * 0.75f);
		AEMtx33Trans(&translation, screenPos.x - (f32)AEGfxGetWindowWidth() * 0.4f, screenPos.y - (f32)AEGfxGetWindowHeight() * 0.25f);

		AEMtx33Concat(&transformation, &rotation, &scale);
		AEMtx33Concat(&transformation, &translation, &transformation);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(npcs[collidedPlayer[0].second].pTexSprite, 0, 0);
		AEGfxSetTransform(transformation.m);
		AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);

		//The box itself
		AEVec2Set(&convBox.position, screenPos.x, screenPos.y - (f32)AEGfxGetWindowHeight() * 0.3725f);
		AEMtx33Trans(&convBox.translation, convBox.position.x, convBox.position.y);
		AEMtx33Concat(&convBox.transformation, &convBox.rotation, &convBox.scale);
		AEMtx33Concat(&convBox.transformation, &convBox.translation, &convBox.transformation);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(convBox.conversationBoxSprite, 0, 0);
		AEGfxSetTransform(convBox.transformation.m);
		AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);

		//Print name and text
		AEGfxPrint(fontID, npcs[collidedPlayer[0].second].npcName, -0.9f, convBox.position.y / (f32)AEGfxGetWindowHeight() - 0.2f, 0.35f, 0.f, 0.f, 0.f, 1.f);
		AEGfxPrint(fontID, npcs[collidedPlayer[0].second].openingText, -0.9f, convBox.position.y / (f32)AEGfxGetWindowHeight() - 0.4f, 0.5f, 0.f, 0.f, 0.f, 1.f);
		break;
	case CONVERSATION_CONTENT: //After pressing Y and enter quest lists / crafting lists
		break;
	case CONVERSATION_EXIT: //Say good bye
		AEMtx33Rot(&rotation, 0.f);
		AEMtx33Scale(&scale, AEGfxGetWindowWidth() * 0.25f, AEGfxGetWindowHeight() * 0.75f);
		AEMtx33Trans(&translation, screenPos.x - (f32)AEGfxGetWindowWidth() * 0.4f, screenPos.y - (f32)AEGfxGetWindowHeight() * 0.25f);

		AEMtx33Concat(&transformation, &rotation, &scale);
		AEMtx33Concat(&transformation, &translation, &transformation);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(npcs[collidedPlayer[0].second].pTexSprite, 0, 0);
		AEGfxSetTransform(transformation.m);
		AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);

		//The box itself
		AEVec2Set(&convBox.position, screenPos.x, screenPos.y - (f32)AEGfxGetWindowHeight() * 0.3725f);
		AEMtx33Trans(&convBox.translation, convBox.position.x, convBox.position.y);
		AEMtx33Concat(&convBox.transformation, &convBox.rotation, &convBox.scale);
		AEMtx33Concat(&convBox.transformation, &convBox.translation, &convBox.transformation);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(convBox.conversationBoxSprite, 0, 0);
		AEGfxSetTransform(convBox.transformation.m);
		AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);

		//Print name and text
		AEGfxPrint(fontID, npcs[collidedPlayer[0].second].npcName, -0.9f, convBox.position.y / (f32)AEGfxGetWindowHeight() - 0.2f, 0.35f, 0.f, 0.f, 0.f, 1.f);
		AEGfxPrint(fontID, npcs[collidedPlayer[0].second].exitingText, -0.9f, convBox.position.y / (f32)AEGfxGetWindowHeight() - 0.4f, 0.5f, 0.f, 0.f, 0.f, 1.f);
		break;
	case CONVERSATION_OUTSIDE: 
	default:
		break;
	}
	
}

void FreeNPC()
{
	for (s8 i = 0; i < 3; i++)
	{
		AEGfxTextureUnload(npcs[i].pTexPortrait);
		AEGfxTextureUnload(npcs[i].pTexSprite);
	}

	AEGfxTextureUnload(convBox.conversationBoxSprite);
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

	void CreateConvBoxInstance(f32 xPos, f32 yPos, f32 xSize, f32 ySize, conversationBox& convBox)
	{
		convBox.rotate = { 0 };
		AEMtx33Rot(&convBox.rotation, 0.f);

		AEVec2Set(&convBox.size, xSize, ySize);
		AEMtx33Scale(&convBox.scale, convBox.size.x, convBox.size.y);

		AEVec2Set(&convBox.position, xPos, yPos);
		AEMtx33Trans(&convBox.translation, convBox.position.x, convBox.position.y);

		convBox.transformation = { 0 };
		AEMtx33Concat(&convBox.transformation, &convBox.rotation, &convBox.scale);
		AEMtx33Concat(&convBox.transformation, &convBox.translation, &convBox.transformation);
		//No need collision box
	}
}