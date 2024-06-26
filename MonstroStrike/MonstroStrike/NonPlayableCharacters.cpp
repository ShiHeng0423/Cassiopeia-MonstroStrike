/*!************************************************************************
  \file					NonPlayableCharacters.cpp
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (85%)
  \secondary author		Teng Shi Heng (10%, helped with collision of NPC), 
						Keith Ng Hon Yew (5%, provided function to check for crafting functions)

  \brief				File containing the implementation of non-playable characters (NPCs)
  
The file include definitions of the functions that include their initialization, updating, drawing, and memory management functions.
Structures for current conversation state, current information regarding to crafting, mission, and checking of collision player are
also included here as well as Functions that create the instances of the entities for NPCs, information displays.  

All content � 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
#include "NonPlayableCharacters.h"
#include "Player.h"
#include <vector>
#include <algorithm>
#include "LevelHeaders.h"
#include "MissionList.h"
#include "Inventory.h"
#include "Crafting.h"


static bool initialMissionsLoaded = false;
//DISCLAIMER: NOTE THAT IT IS ONLY 3 NPC NOW UNLESS SUBJECT TO CHANGES

namespace
{
	struct NonPlayableCharacters npcs[3];

	void CreateNPCInstance(f32 xPos, f32 yPos, f32 xSize, f32 ySize, NonPlayableCharacters& npc, NpcTypes npcType);

	void CreateConvBoxInstance(f32 xPos, f32 yPos, f32 xSize, f32 ySize, ConversationContent& convBox);

	void CreateContentBarInstance(int num);

	void CreateInfoDisplayBanner();

	struct ConversationContent convBox;

	AEGfxTexture* npcContentBox;
	AEGfxTexture* contentListBox; //The list of bars shown in content, like showing missions, crafting lists etc...
	AEGfxTexture* infoDisplayBoxSprite;
	AEGfxTexture* interactionButton;


	enum ConversationState
	{
		CONVERSATION_OUTSIDE = 0,
		CONVERSATION_ENTRY,
		CONVERSATION_CONTENT,
		CONVERSATION_EXIT

	} currentConvState;

	AEVec2 screenPos;

	AEVec2 mousePos{0, 0};

	f32 yScale;


	std::vector<std::pair<f32, s16>> collidedPlayer;

	std::vector<int> availableMissionsID;

	std::vector<struct ContentBar> contentBarContainer;

	struct ContentBar infoDisplayBox;

	bool displayBoxActive = false;
	bool confirmAcceptPrompt = false;
	bool checkMissionClear = false;

	//For items
	int loc1 = -1;
	int loc2 = -1;

	struct HoverContentInfo
	{
		const char* hoverContentName = {};
		const char* hoverMissionDetails = {};
		std::vector<std::string> rewardsDetails = {};

		int missionID = -1;

		
		Recipe theRecipe = {};
	} currentMissionInfo, currentCraftingInfo;

	const char* confirmText;

	const Recipe* recipePtr = nullptr;
	const KillEnemyMission* missionPtr = nullptr;
}

void LoadNPC()
{
	//Texture loading
	npcs[0].pTexPortrait = AEGfxTextureLoad("Assets/NPCs/NPC_Cleric_Portrait.png");
	npcs[1].pTexPortrait = AEGfxTextureLoad("Assets/NPCs/NPC_Blacksmith_B_Portrait.png");
	npcs[2].pTexPortrait = AEGfxTextureLoad("Assets/NPCs/NPC_QuestReceptionist_Portrait.png");

	npcs[0].pTexSprite = AEGfxTextureLoad("Assets/NPCs/NPC_Cleric_Portrait.png");
	npcs[1].pTexSprite = AEGfxTextureLoad("Assets/NPCs/NPC_Blacksmith_B_Portrait.png");
	npcs[2].pTexSprite = AEGfxTextureLoad("Assets/NPCs/NPC_QuestReceptionist_Portrait.png");

	convBox.conversationBoxSprite = AEGfxTextureLoad("Assets/UI_Sprite/NPC_Conv/ConversationBox.png");
	npcContentBox = AEGfxTextureLoad("Assets/UI_Sprite/NPC_Conv/NPC_ContentScreen.png");
	contentListBox = AEGfxTextureLoad("Assets/UI_Sprite/NPC_Conv/Contentbar.png");
	infoDisplayBoxSprite = AEGfxTextureLoad("Assets/UI_Sprite/NPC_Conv/InfoContentBanner.png");

	interactionButton = AEGfxTextureLoad("Assets/Keyboard_Keys/keyboard_i.png");

	if (!initialMissionsLoaded)
	{
		missionSystem.InitialMission();
		initialMissionsLoaded = true;
	}
}

void InitializeNPC(std::vector<AEVec2> allocatedPositions)
{
	for (int i = 0; i < allocatedPositions.size(); i++)
	{
		AEVec2 pos = allocatedPositions[i];
		switch (i)
		{
		case 0:
			CreateNPCInstance(pos.x, pos.y * 0.96f, 70.f, 70.f, npcs[i], NPC_CLERIC);
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

	//Initialize the names, texts etc
	npcs[0].npcName = "Alice, Cleric";
	npcs[1].npcName = "Jack, Weapon Blacksmith";
	npcs[2].npcName = "Marie, Quest Counter";


	npcs[0].openingText = "Do you require some healing? [Y/N]";
	npcs[1].openingText = "It is too dangerous to go alone, craft some weapons from me.[Y/N]";
	npcs[2].openingText = "Any quests that caught your eye? [Y/N]";

	npcs[0].exitingText = "May lord be with you. [Y] to continue";
	npcs[1].exitingText = "Feel free to come back anytime. [Y] to continue";
	npcs[2].exitingText = "See you... [Y] to continue";

	confirmText = "Are you sure?";

	displayBoxActive = false;
}

void UpdateNPC(Player* player)
{
	collidedPlayer.clear(); //Clear vector
	//By Shi heng
	for (s16 i = 0; i < 3; i++) //Check collided NPC
	{
		switch (npcs[i].typeOfNPC)
		{
		case NPC_CLERIC:
			if (AABBvsAABB(player->GetPlayerCollisionBox(), npcs[i].collisionBox))
			{
				collidedPlayer.push_back({AEVec2Distance(&player->GetPlayerCurrentPosition(), &npcs[i].position), i});
			}
			break;
		case NPC_BLACKSMITH_B:
			if (AABBvsAABB(player->GetPlayerCollisionBox(), npcs[i].collisionBox))
			{
				collidedPlayer.push_back({AEVec2Distance(&player->GetPlayerCurrentPosition(), &npcs[i].position), i});
			}
			break;
		case NPC_QUEST_GIVER:
			if (AABBvsAABB(player->GetPlayerCollisionBox(), npcs[i].collisionBox))
			{
				collidedPlayer.push_back({AEVec2Distance(&player->GetPlayerCurrentPosition(), &npcs[i].position), i});
			}
			break;
		default:
			std::cout << "NPC " << i << " does not have a type?\n";
			break;
		}
	}

	//Check if there are any collided NPC
	if (AEInputCheckTriggered(AEVK_I) && !collidedPlayer.empty() && !player->GetIsTalkingToNpc())
	{
		std::sort(collidedPlayer.begin(), collidedPlayer.end()); //Sort the closest collided NPC to be front
		currentConvState = CONVERSATION_ENTRY;
		player->GetIsTalkingToNpc() = true;
	}

	if (player->GetIsTalkingToNpc())
	{
		switch (currentConvState)
		{
		case CONVERSATION_ENTRY:
			if (missionSystem.CheckMissionClear())
			{
				npcs[2].openingText = "Congrats! You completed the mission! [Y/N]";
			}
			else
			{
				npcs[2].openingText = "Any quests that caught your eye? [Y/N]";
			}

			if (AEInputCheckTriggered(AEVK_Y))
			{
				switch (collidedPlayer[0].second)
				{
				case NPC_CLERIC:
					//Heals the player
					player->GetCurrentHealth() = player->GetMaxHealth();
				//Plays healing sound effect
					currentConvState = CONVERSATION_EXIT;
					break;
				case NPC_BLACKSMITH_B:
					//Activate next conversation
					currentConvState = CONVERSATION_CONTENT;
					contentBarContainer.clear();
					for (int i = 0; i < Crafting::recipeList.size(); i++)
					{
						CreateContentBarInstance(i);
					}
					break;
				case NPC_QUEST_GIVER:
					if (missionSystem.CheckMissionClear())
					{
						missionSystem.MissionComplete((int)missionSystem.GetAcceptedMissionID());
					}
				//Activate next conversation
					currentConvState = CONVERSATION_CONTENT;
					contentBarContainer.clear(); //Clean container
					availableMissionsID = missionSystem.GetAvailableEnemyMissionsIDs();
					for (int i = 0; i < availableMissionsID.size(); i++)
					{
						CreateContentBarInstance(i);
					}
					break;
				default:
					break;
				}
			}
			else if (AEInputCheckTriggered(AEVK_N))
			{
				currentConvState = CONVERSATION_EXIT;
			}
			break;
		case CONVERSATION_CONTENT:
			//Check which content to display
			s32 x, y;
			AEInputGetCursorPosition(&x, &y);
			mousePos.x = screenPos.x - AEGfxGetWindowWidth() * 0.5f + x;
			mousePos.y = screenPos.y + AEGfxGetWindowHeight() * 0.5f - y;
			switch (collidedPlayer[0].second)
			{
			case NPC_CLERIC:
				break;
			case NPC_BLACKSMITH_B:
				if (!confirmAcceptPrompt)
				{
					displayBoxActive = false;

					for (int i = 0; i < Crafting::recipeList.size(); i++)
					{
						for (const Recipe& recipe : Crafting::recipeList)
						{
							if (recipe.item_id == Crafting::recipeList[i].item_id)
							{
								recipePtr = &recipe;
								break;
							}
						}

						if (AETestPointToRect(&mousePos, &contentBarContainer[i].position,
						                      contentBarContainer[i].size.x, contentBarContainer[i].size.y))
						{
							displayBoxActive = true;
							if (recipePtr == nullptr)
								return;

							CreateInfoDisplayBanner(); //Update

							currentCraftingInfo.theRecipe = *recipePtr;
							if (AEInputCheckTriggered(AEVK_LBUTTON))
							{
								 for (int j = 0; j < playerInventory.size(); j++)
								 {
								 	if (playerInventory[j].ID == recipePtr->mat_requirements.first.mat_ID)
								 	{
								 		loc1 = j;
								 	}
								 	if (playerInventory[j].ID == recipePtr->mat_requirements.second.mat_ID)
								 	{
								 		loc2 = j;
								 	}
								 }

								if (Crafting::Can_Craft(*recipePtr, playerInventory, loc1, loc2))
								{
									confirmAcceptPrompt = true;
								}
								else
								{
									//Play Unavailable sound effect here, something like DE DEEE
									std::cout << "Unavailable\n";
									audioManager->PlayAudio(false, REJECT_SFX);
								}
							}
						}
					}
				}
				else
				{
					if (AEInputCheckTriggered(AEVK_Y))
					{
						Crafting::Craft_Item(currentCraftingInfo.theRecipe, playerInventory, loc1, loc2);

						//Reset the content bars...
						contentBarContainer.clear();
						for (int j = 0; j < Crafting::recipeList.size(); j++)
						{
							CreateContentBarInstance(j);
						}
						confirmAcceptPrompt = false;
					}
					else if (AEInputCheckTriggered(AEVK_N))
					{
						confirmAcceptPrompt = false;
					}
				}
				break;
			case NPC_QUEST_GIVER:
				if (!confirmAcceptPrompt)
				{
					displayBoxActive = false;
					const KillEnemyMission* theMission = nullptr;

					for (int i = 0; i < availableMissionsID.size(); i++)
					{
						for (const KillEnemyMission& mission : missionSystem.enemyMissions)
						{
							if (mission.missionID == availableMissionsID[i])
							{
								theMission = &mission;
								break;
							}
						}

						if (AETestPointToRect(&mousePos, &contentBarContainer[i].position,
						                      contentBarContainer[i].size.x, contentBarContainer[i].size.y))
						{
							displayBoxActive = true;
							if (theMission == nullptr)
								return;

							currentMissionInfo.hoverContentName = theMission->missionName;
							currentMissionInfo.hoverMissionDetails = theMission->missionDetails;
							currentMissionInfo.rewardsDetails = theMission->rewardDetails;

							CreateInfoDisplayBanner(); //Update

							if (AEInputCheckTriggered(AEVK_LBUTTON))
							{
								if (missionSystem.GetAcceptedMissionID() == -1)
								{
									currentMissionInfo.missionID = theMission->missionID;
									confirmAcceptPrompt = true;
								}
								else
								{
									//Play Unavailable sound effect here, something like DE DEEE
									audioManager->PlayAudio(false, REJECT_SFX);
									std::cout << "Unavailable\n";
								}
							}
						}
					}
				}
				else
				{
					if (AEInputCheckTriggered(AEVK_Y))
					{
						missionSystem.AcceptKillEnemyMission((int)currentMissionInfo.missionID);
						//Reset the content bars...
						availableMissionsID = missionSystem.GetAvailableEnemyMissionsIDs();
						if (!availableMissionsID.empty())
						{
							contentBarContainer.clear();
							for (int i = 0; i < availableMissionsID.size(); i++)
							{
								CreateContentBarInstance(i);
							}
						}
						confirmAcceptPrompt = false;
					}
					else if (AEInputCheckTriggered(AEVK_N))
					{
						confirmAcceptPrompt = false;
					}
				}
				break;
			}
			if (AEInputCheckTriggered(AEVK_ESCAPE) && !confirmAcceptPrompt)
			{
				currentConvState = CONVERSATION_EXIT;
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
			player->GetIsTalkingToNpc() = false;
			displayBoxActive = false;
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

	if (!collidedPlayer.empty())
	{
		AEMtx33Scale(&scale, GRID_SIZE, GRID_SIZE);
		AEMtx33Rot(&rotation, 0.f);
		AEMtx33Trans(&translation, npcs[collidedPlayer[0].second].position.x,
		             npcs[collidedPlayer[0].second].position.y +
		             npcs[collidedPlayer[0].second].size.y);
		AEMtx33Concat(&transformation, &rotation, &scale);
		AEMtx33Concat(&transformation, &translation, &transformation);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(interactionButton, 0, 0);
		AEGfxSetTransform(transformation.m);
		AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);
	}

	if (inConv)
	{
		//Drawing NPC portrait
		AEMtx33Rot(&rotation, 0.f);
		AEMtx33Scale(&scale, AEGfxGetWindowWidth() * 0.25f, AEGfxGetWindowHeight() * 0.75f);
		AEMtx33Trans(&translation, screenPos.x - (f32)AEGfxGetWindowWidth() * 0.4f,
		             screenPos.y - (f32)AEGfxGetWindowHeight() * 0.25f);

		AEMtx33Concat(&transformation, &rotation, &scale);
		AEMtx33Concat(&transformation, &translation, &transformation);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(npcs[collidedPlayer[0].second].pTexSprite, 0, 0);
		AEGfxSetTransform(transformation.m);
		AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);
	}

	switch (currentConvState)
	{
	case CONVERSATION_ENTRY:
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
		AEGfxPrint(fontID, npcs[collidedPlayer[0].second].npcName, -0.9f,
		           convBox.position.y / (f32)AEGfxGetWindowHeight() - 0.2f, 0.35f, 0.f, 0.f, 0.f, 1.f); //name 
		AEGfxPrint(fontID, npcs[collidedPlayer[0].second].openingText, -0.9f,
		           convBox.position.y / (f32)AEGfxGetWindowHeight() - 0.4f, 0.5f, 0.f, 0.f, 0.f, 1.f); //text
		break;
	case CONVERSATION_CONTENT: //After pressing Y and enter quest lists / crafting lists

		//Content box itself
		AEMtx33Rot(&rotation, 0.f);
		AEMtx33Scale(&scale, (f32)AEGfxGetWindowWidth() * 0.8f, (f32)AEGfxGetWindowHeight());
		AEMtx33Trans(&translation, screenPos.x - (f32)AEGfxGetWindowWidth() * 0.1f, screenPos.y);

		AEMtx33Concat(&transformation, &rotation, &scale);
		AEMtx33Concat(&transformation, &translation, &transformation);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(npcContentBox, 0, 0);
		AEGfxSetTransform(transformation.m);
		AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);

		switch (collidedPlayer[0].second)
		{
		case NPC_CLERIC:
			//Available recipes
			break;
		case NPC_BLACKSMITH_B:
			for (int i = 0; i < Crafting::recipeList.size(); i++)
			{
				yScale = (f32)AEGfxGetWindowHeight() * 0.075f;

				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxTextureSet(contentListBox, 0, 0);
				AEGfxSetTransform(contentBarContainer[i].transformation.m);
				AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);

				/*	for (const Recipe& recipe : Crafting::recipeList)
					{
						if (recipe.item_id == Crafting::recipeList[i].item_id)
						{
							recipePtr = &recipe;
							break;
						}
					}*/

				if (recipePtr)
				{
					AEVec2 posText = {
						-0.5f,
						(screenPos.y + (f32)AEGfxGetWindowHeight() - yScale * 0.8f) - yScale * i * 2.25f + GRID_SIZE * 2
					};
					AEGfxPrint(fontID, fullInventoryList[Crafting::recipeList[i].item_id].name.c_str(), posText.x,
					           (posText.y / AEGfxGetWindowHeight()), 0.35f, 0.f, 0.f, 0.f, 1.f);
				}
			}

			if (currentCraftingInfo.theRecipe.item_id >= 0 && displayBoxActive)
			{
				AEGfxTextureSet(infoDisplayBoxSprite, 0, 0);
				AEGfxSetTransform(infoDisplayBox.transformation.m);
				AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);

				std::string firstMatName = fullInventoryList[currentCraftingInfo.theRecipe.mat_requirements.first.
					mat_ID].name;
				firstMatName += " x" +
					std::to_string(currentCraftingInfo.theRecipe.mat_requirements.first.mat_quantity);

				//To test if item name match
				//std::string secondMatName = fullInventoryList[currentCraftingInfo.theRecipe.item_id].name;

				std::string secondMatName = fullInventoryList[currentCraftingInfo.theRecipe.mat_requirements.second.
					mat_ID].name;


				secondMatName += " x" + std::to_string(
					currentCraftingInfo.theRecipe.mat_requirements.second.mat_quantity);

				AEGfxPrint(fontID, "MATERIALS", 0.7f, 0.8f, 0.35f, 0.f, 0.f, 0.f, 1.f);
				AEGfxPrint(fontID, firstMatName.c_str(), 0.625f, 0.7f, 0.3f, 0.f, 0.f, 0.f, 1.f);
				AEGfxPrint(fontID, secondMatName.c_str(), 0.625f, 0.5f, 0.3f, 0.f, 0.f, 0.f, 1.f);
			}
		//Available recipes
			break;
		case NPC_QUEST_GIVER:

			for (int i = 0; i < availableMissionsID.size(); i++) //Should draw maximum number of boxes...
			{
				yScale = (f32)AEGfxGetWindowHeight() * 0.075f;

				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxTextureSet(contentListBox, 0, 0);
				AEGfxSetTransform(contentBarContainer[i].transformation.m);
				AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);

				for (const KillEnemyMission& mission : missionSystem.enemyMissions)
				{
					if (mission.missionID == availableMissionsID[i])
					{
						missionPtr = &mission;
						break;
					}
				}
				if (missionPtr)
				{
					AEVec2 posText = {
						-0.5f,
						(screenPos.y + (f32)AEGfxGetWindowHeight() - yScale * 0.8f) - yScale * i * 2.25f + GRID_SIZE * 2
					};
					AEGfxPrint(fontID, missionPtr->missionName, posText.x, (posText.y / AEGfxGetWindowHeight()), 0.35f,
					           0.f, 0.f, 0.f, 1.f);
				}
			}

			if (displayBoxActive)
			{
				AEGfxTextureSet(infoDisplayBoxSprite, 0, 0);
				AEGfxSetTransform(infoDisplayBox.transformation.m);
				AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);

				AEGfxPrint(fontID, "QUEST INFO", 0.7f, 0.8f, 0.35f, 0.f, 0.f, 0.f, 1.f);
				AEGfxPrint(fontID, currentMissionInfo.hoverMissionDetails, 0.63f, 0.f, 0.3f, 0.f, 0.f, 0.f, 1.f);
				
				AEGfxPrint(fontID, "REWARDS", 0.7f, -0.1f, 0.35f, 0.f, 0.f, 0.f, 1.f);
				f32 yOffset = -0.2f;
				for (const std::string& reward : currentMissionInfo.rewardsDetails) {
					AEGfxPrint(fontID, reward.c_str(), 0.63f, yOffset, 0.28f, 0.f, 0.f, 0.f, 1.f);
					yOffset -= 0.1f;
				}
			}
			break;
		}

		if (!confirmAcceptPrompt)
		{
			//Default for everyone else
			AEGfxPrint(fontID, "Anything fancy?", -0.98f, -0.7f, 0.35f, 0.f, 0.f, 0.f, 1.f);
			//If possible want to improve, doesn't really like hardcoded values with no scalar...
			AEGfxPrint(fontID, "[Esc] to exit", -0.98f, -0.8f, 0.35f, 0.f, 0.f, 0.f, 1.f);
		}
		else
		{
			AEGfxPrint(fontID, "Are you sure?", -0.98f, -0.7f, 0.35f, 0.f, 0.f, 0.f, 1.f);
			AEGfxPrint(fontID, "[Y] to accept", -0.98f, -0.8f, 0.35f, 0.f, 0.f, 0.f, 1.f);
			AEGfxPrint(fontID, "[N] to reject", -0.98f, -0.9f, 0.35f, 0.f, 0.f, 0.f, 1.f);
		}
		break;
	case CONVERSATION_EXIT: //Say good bye
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
		AEGfxPrint(fontID, npcs[collidedPlayer[0].second].npcName, -0.9f,
		           convBox.position.y / (f32)AEGfxGetWindowHeight() - 0.2f, 0.35f, 0.f, 0.f, 0.f, 1.f);
		AEGfxPrint(fontID, npcs[collidedPlayer[0].second].exitingText, -0.9f,
		           convBox.position.y / (f32)AEGfxGetWindowHeight() - 0.4f, 0.5f, 0.f, 0.f, 0.f, 1.f);
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
	AEGfxTextureUnload(contentListBox);
	AEGfxTextureUnload(npcContentBox);
	AEGfxTextureUnload(infoDisplayBoxSprite);
	AEGfxTextureUnload(interactionButton);

	collidedPlayer.clear();
	availableMissionsID.clear();
	contentBarContainer.clear();
}

namespace
{
	void CreateNPCInstance(f32 xPos, f32 yPos, f32 xSize, f32 ySize, NonPlayableCharacters& npc, NpcTypes npcType)
	{
		npc.rotation = {0};
		AEMtx33Rot(&npc.rotation, 0.f);

		AEVec2Set(&npc.size, xSize, ySize);
		AEMtx33Scale(&npc.scale, npc.size.x, npc.size.y);

		AEVec2Set(&npc.position, xPos, yPos);
		AEMtx33Trans(&npc.translation, npc.position.x, npc.position.y);

		npc.transformation = {0};
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

	void CreateConvBoxInstance(f32 xPos, f32 yPos, f32 xSize, f32 ySize, ConversationContent& boxObj)
	{
		boxObj.rotate = {0};
		AEMtx33Rot(&boxObj.rotation, 0.f);

		AEVec2Set(&boxObj.size, xSize, ySize);
		AEMtx33Scale(&boxObj.scale, boxObj.size.x, boxObj.size.y);

		AEVec2Set(&boxObj.position, xPos, yPos);
		AEMtx33Trans(&boxObj.translation, boxObj.position.x, boxObj.position.y);

		boxObj.transformation = {0};
		AEMtx33Concat(&boxObj.transformation, &boxObj.rotation, &boxObj.scale);
		AEMtx33Concat(&boxObj.transformation, &boxObj.translation, &boxObj.transformation);
		//No need collision box
	}

	void CreateContentBarInstance(int num)
	{
		ContentBar contBar = {};
		f32 ySize = (f32)AEGfxGetWindowHeight() * 0.075f;

		AEMtx33Rot(&contBar.rotation, 0.f);

		AEVec2Set(&contBar.size, (f32)AEGfxGetWindowWidth() * 0.55f, ySize);
		AEMtx33Scale(&contBar.scale, contBar.size.x, contBar.size.y);

		AEVec2Set(&contBar.position, screenPos.x * 1.02f,
		          (screenPos.y + (f32)AEGfxGetWindowHeight() * 0.5f - ySize * 0.8f) - ySize * num * 1.1f);
		AEMtx33Trans(&contBar.translation, contBar.position.x, contBar.position.y);

		contBar.transformation = {0};
		AEMtx33Concat(&contBar.transformation, &contBar.rotation, &contBar.scale);
		AEMtx33Concat(&contBar.transformation, &contBar.translation, &contBar.transformation);

		//AABB box
		contBar.collisionBox.minimum.x = contBar.position.x - contBar.size.x * 0.5f;
		contBar.collisionBox.minimum.y = contBar.position.y - contBar.size.y * 0.5f;
		contBar.collisionBox.maximum.x = contBar.position.x + contBar.size.x * 0.5f;
		contBar.collisionBox.maximum.y = contBar.position.y + contBar.size.y * 0.5f;

		contentBarContainer.push_back(contBar);
	}

	void CreateInfoDisplayBanner()
	{
		AEMtx33Rot(&infoDisplayBox.rotation, 0.f);

		AEVec2Set(&infoDisplayBox.size, (f32)AEGfxGetWindowWidth() * 0.18f, (f32)AEGfxGetWindowHeight() * 0.95f);
		AEMtx33Scale(&infoDisplayBox.scale, infoDisplayBox.size.x, infoDisplayBox.size.y);

		AEVec2Set(&infoDisplayBox.position, screenPos.x + (f32)AEGfxGetWindowWidth() * 0.4f,
		          screenPos.y);
		AEMtx33Trans(&infoDisplayBox.translation, infoDisplayBox.position.x, infoDisplayBox.position.y);

		infoDisplayBox.transformation = {0};
		AEMtx33Concat(&infoDisplayBox.transformation, &infoDisplayBox.rotation, &infoDisplayBox.scale);
		AEMtx33Concat(&infoDisplayBox.transformation, &infoDisplayBox.translation, &infoDisplayBox.transformation);
	}
}
