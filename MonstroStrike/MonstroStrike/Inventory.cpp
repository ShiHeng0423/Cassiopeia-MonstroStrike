/*!*****************************************************************************
\file				Inventory.cpp
\project name		Monstrostrike
\primary author		Keith Ng Hon Yew (100%)
\brief				This file contains the definition of all functions related
					to the inventory system.

All content � 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
*******************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "Inventory.h"
#include "Utils.h"

#include <algorithm>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <cstdio>
#include <vector>

#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/filewritestream.h"
#include <cstdio>
#include "rapidjson/writer.h"
#include <fstream>
#include <sstream>

#include "Crafting.h"
#include "GameStateManager.h"
#include "main.h"

using namespace rapidjson;

 std::vector< Item> playerInventory;
 std::vector< Item> fullInventoryList;

 AEGfxTexture* Gear[29];
 s16 snapBack = -1;

 Player* playerReference;
 std::vector< Item> equippedGear;

 ButtonGearUI inventoryBackground;
 ButtonGearUI inventoryButton[MAX_INVENTORY_SIZE];

 ButtonGearUI item_background[MAX_INVENTORY_SIZE];

 ButtonGearUI equipmentBackground;
 ButtonGearUI itemDisplayBackground;

 AEGfxTexture* blank;

 int playerInventoryCount;

namespace Inventory
{
	//Global Variable
	bool isNewAccount;
	bool isGodAccount = false;
	GameStates fileLoadedState;
	
	std::vector<Item> allItems; //list of all items in game
	ButtonGearUI equipmentDisplay[5];
	bool inventoryOpen;
	bool itemHover;
	ButtonGearUI displayItem;

	Item backupPreviousItem[5];

	Item emptySpace{ "", INVALID_ITEM, "", "",
IT_NONE, IR_NONE, GL_NONE, 0,
false, 0, 0, 0 };



	std::vector< Item> ReadJsonFile(const std::string& filepath)
	{
		std::vector<Item> inventory;

		std::ifstream ifs(filepath);
		if (!ifs.is_open())
		{
			std::cerr << "Failed to open the JSON file" << std::endl;
			return inventory;
		}
		else
		{
			Document json;
			IStreamWrapper isw(ifs);
			json.ParseStream(isw);

			// Check if the document is valid 
			if (json.HasParseError())
			{
				std::cerr << "Error: failed to parse JSON document"
					<< std::endl;
				ifs.close();
			}

			std::cout << "Access items in " << filepath << std::endl;
			assert(json.IsObject());

			//Load the map location for player
			assert(json["map"].IsInt());

			if (filepath == "Assets/SaveFiles/player_inventory.json")
			{
				fileLoadedState = static_cast<GameStates> (json["map"].GetInt());
				std::cout << "Previous Map loaded from file: " << fileLoadedState << std::endl;
			}

			assert(json["items"].IsArray());
			const Value& items = json["items"];

			//Check if items list is empty
			if (items.Size() == 0)
				return inventory;

			for (SizeType loc = 0; loc < items.Size(); loc++)
			{
				const Value& ind_item = items[loc];

				Item newItem;

				newItem.UID = ind_item["UID"].GetString();
				newItem.ID = ind_item["ID"].GetInt();
				newItem.name = ind_item["name"].GetString();
				newItem.description = ind_item["description"].GetString();
				newItem.item_type = static_cast<Item_Type>(ind_item["item_type"].GetInt());
				newItem.rarity = static_cast<Rarity> (ind_item["rarity"].GetInt());
				newItem.gear_loc = static_cast<Gear_Location> (ind_item["gear_location"].GetInt());
				newItem.quantity = ind_item["quantity"].GetInt();
				newItem.stackable = ind_item["stackable"].GetBool();
				newItem.health = ind_item["health"].GetInt();
				newItem.attack = ind_item["attack"].GetInt();
				newItem.defence = ind_item["defence"].GetInt();

				if(newItem.ID>=0)
				inventory.push_back(newItem);

			}

			if (json.HasParseError())
			{
				std::cerr << "Error parsing JSON: " << std::endl;
			}
		}
		return inventory;
	}

	void WriteJsonFile(const std::vector<Item>& inventory, const std::string& filepath)
	{
		std::cout << "start writing JSON" << std::endl;

		 Document json = nullptr;
		 json.SetObject();

		Value items( kArrayType);

		for (const auto& item : inventory)
		{
			//Skip empty inventory slots
			if(item.ID < 0)
				continue;

			Value ind_item(kObjectType);
			ind_item.AddMember("UID", Value(item.UID.c_str(), json.GetAllocator()), json.GetAllocator());
			ind_item.AddMember("ID", item.ID, json.GetAllocator());
			ind_item.AddMember("name", Value(item.name.c_str(), json.GetAllocator()), json.GetAllocator());
			ind_item.AddMember("description", Value(item.description.c_str(), json.GetAllocator()),
			                   json.GetAllocator());
			ind_item.AddMember("item_type", item.item_type, json.GetAllocator());
			ind_item.AddMember("rarity", item.rarity, json.GetAllocator());
			ind_item.AddMember("gear_location", item.gear_loc, json.GetAllocator());
			ind_item.AddMember("quantity", item.quantity, json.GetAllocator());
			ind_item.AddMember("stackable", item.stackable, json.GetAllocator());
			ind_item.AddMember("health", item.health, json.GetAllocator());
			ind_item.AddMember("attack", item.attack, json.GetAllocator());
			ind_item.AddMember("defence", item.defence, json.GetAllocator());

			items.PushBack(ind_item, json.GetAllocator());
		}
		std::cout << "Current map SAVED to file: " << current << std::endl;

		json.AddMember("map", current, json.GetAllocator());

		json.AddMember("items", items, json.GetAllocator());



		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);

		json.Parse(buffer.GetString());

		json.Accept(writer);
		
		 std::ofstream ofs;

		 ofs.open(filepath,std::fstream::out);

		if (ofs.is_open())
		{
			ofs << buffer.GetString();
			std::cout << "Successfully wrote to the JSON file: " << filepath << std::endl;
		}
		else
		{
			std::cerr << "Failed to open the output JSON file: " << filepath << std::endl;
		}

		ofs.close();

	}



	//Update inventory vector every frame
	void UpdateInventory(const std::vector<Item>& inventory, ButtonGearUI button[])
	{
		
		int count = 0;

		for(size_t i=0; i< playerInventory.size(); ++i)
		{
			if (playerInventory[i].quantity <= 0)
			{
				playerInventory[i] = emptySpace;
			}

			button[i].Item = playerInventory[i];

			if(playerInventory[i].ID >= 0)
			{
				count++;
			}
		}

		

		if(inventory.size()<25)
		{
			for(size_t i = inventory.size() ; i < 25 ; i++)
			{
				button[i].Item = emptySpace;
			}
		}

		playerInventoryCount = count;
	}

	void UpdateInventoryUI()
	{
		s16 index = 0;
		s32 textX = 0;
		s32 textY = 0;


		index = 0;
		AEInputGetCursorPosition(&textX, &textY);
		AEVec2 mousePos = {};
		mousePos.x = textX - AEGfxGetWindowWidth() * 0.5f;
		mousePos.y = AEGfxGetWindowHeight() * 0.5f - textY;

		for (ButtonGearUI& button : inventoryButton)
		{
			if (AETestPointToRect(&mousePos, &button.pos, button.img.scale.x, button.img.scale.y))
			{
				if (button.img.pTex != blank)
				{
					UseItem(index, button, *playerReference);
					if (button.Item.quantity == 0 || button.Item.ID < 0)
					{
						button.img.pTex = blank;
						button.Item = emptySpace;
					}
					break;
				}
			}
			index++;
		}


		//Update equipped item UI
		index = 0;
		for (ButtonGearUI& button : equipmentDisplay)
		{
			if (button.Item.ID < 0)
			{
				button.img.pTex = blank;
			}
			else
			{
				button.img.pTex = Gear[button.Item.ID];
			}
			//Set scale
			AEVec2Set(&button.img.scale, 90.f, 90.f);
			index++;
		}
		//Set custom position
		AEVec2Set(&equipmentDisplay[0].pos, -550.f, 30.f);
		AEVec2Set(&equipmentDisplay[1].pos, -350.f, 30.f);

		AEVec2Set(&equipmentDisplay[2].pos, -450.f, -65.f);

		AEVec2Set(&equipmentDisplay[3].pos, -550.f, -160.f);
		AEVec2Set(&equipmentDisplay[4].pos, -350.f, -160.f);

	}

	void UpdateEquipmentUI()
	{
		s16 index = 0;
		s32 textX = 0;
		s32 textY = 0;


		index = 0;
		AEInputGetCursorPosition(&textX, &textY);
		AEVec2 mousePos = {};
		mousePos.x = textX - AEGfxGetWindowWidth() * 0.5f;
		mousePos.y = AEGfxGetWindowHeight() * 0.5f - textY;

		//Unequip Item
		for (ButtonGearUI& equipment_slot : equipmentDisplay)
		{
			if (AETestPointToRect(&mousePos, &equipment_slot.pos, equipment_slot.img.scale.x, equipment_slot.img.scale.y))
			{
				if(equipment_slot.Item.ID >=0)
				{
					Item backup = equipment_slot.Item;

					UnequipItem(equipment_slot.Item.gear_loc);

					AddItem(backup);
				}

			}
		}


		//Update equipped item UI
		index = 0;
		for (ButtonGearUI& button : equipmentDisplay)
		{
			if (button.Item.ID < 0)
			{
				button.img.pTex = blank;
			}
			else
			{
				button.img.pTex = Gear[button.Item.ID];
			}
			//Set scale
			AEVec2Set(&button.img.scale, 90.f, 90.f);
			index++;
		}
		//Set custom position
		AEVec2Set(&equipmentDisplay[0].pos, -550.f, 30.f);
		AEVec2Set(&equipmentDisplay[1].pos, -350.f, 30.f);

		AEVec2Set(&equipmentDisplay[2].pos, -450.f, -65.f);

		AEVec2Set(&equipmentDisplay[3].pos, -550.f, -160.f);
		AEVec2Set(&equipmentDisplay[4].pos, -350.f, -160.f);

	}


	void SwapInventory(Item& lhs, Item& rhs)
	{
		Item tmp = lhs;
		lhs = rhs;
		rhs = tmp;

		audioManager->PlayAudio(false, Audio_List::ITEM_SWAP);
	}

	// Function to get an item by its ID
	Item GetItemById(int id) {
		// Iterate through all items to find the one with the matching ID
		for (const Item& item : allItems) {
			if (item.ID == id) {
				return item;
			}
		}
		// Return a default item if the ID is not found
		return Item{ "Unknown Item", -1, "Forbidden Fruit", "So black, it can't be seen, ever...", FOOD,UNIQUE,GL_NONE, true, 1,1 };
	}



	void OpenInventory()
	{

		s16 index = 0;
		for (ButtonGearUI& button : inventoryButton)
		{
			button.img.pTex = blank;
			AEVec2Set(&button.img.scale, 60.f, 60.f);
			AEVec2Set(&button.pos, (int)(index % 5) * 90.f - 180.f, (int)-(index / 5.f) * 90.f + 180.f);

			AEVec2Set(&item_background[index].img.scale, 60.f, 60.f);
			AEVec2Set(&item_background[index].pos, (int)(index % 5) * 90.f - 180.f, (int)-(index / 5.f) * 90.f + 180.f);

			if (button.Item.quantity <= 0)
			{
				button.Item = emptySpace;
			}
			else
			{
				button.img.pTex = Gear[button.Item.ID];
			}
			index++;
		}
	
		//Hover collision with button && hold left mouse button
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			s32 textX = 0;
			s32 textY = 0;

			s16 indexTmp = 0;

			AEInputGetCursorPosition(&textX, &textY);
			AEVec2 mousePos = {};
			mousePos.x = textX - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - textY;

			for (ButtonGearUI& button : inventoryButton)
			{
				if (AETestPointToRect(&mousePos, &button.pos, button.img.scale.x, button.img.scale.y))
				{
					if (button.Item.ID >= 0)
					{
						//snap origin of img to mouse pos
						snapBack = indexTmp;

						//Display item's info on l_click
						itemHover = true;
						displayItem = button;

						audioManager->PlayAudio(false, Audio_List::ITEM_CLICK);

						break;
					}
				}
				//Reset itemHover
				itemHover = false;
				indexTmp++;
			}


			for (ButtonGearUI& button : equipmentDisplay)
			{
				if (AETestPointToRect(&mousePos, &button.pos, button.img.scale.x, button.img.scale.y))
				{
					if (button.Item.ID >= 0)
					{

						//Display item's info on l_click
						itemHover = true;
						displayItem = button;
						displayItem.img.scale.x = inventoryButton->img.scale.x;
						displayItem.img.scale.y = inventoryButton->img.scale.y;

						audioManager->PlayAudio(false, Audio_List::ITEM_CLICK);

						break;
					}
				}
			}
		}

		//Drag Item with mouse logic
		if (snapBack >= 0)
		{
			s32 textX = 0;
			s32 textY = 0;

			//s16 index = 0;

			AEInputGetCursorPosition(&textX, &textY);
			AEVec2 mousePos = {};
			mousePos.x = textX - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - textY;

			inventoryButton[snapBack].pos = mousePos;
		}

		if (AEInputCheckReleased(AEVK_LBUTTON))
		{
			s16 indexTmp = 0;
			if (snapBack >= 0)
			{
				for (ButtonGearUI& button : inventoryButton)
				{
					if (AETestRectToRect(&inventoryButton[snapBack].pos,
						inventoryButton[snapBack].img.scale.x,
						inventoryButton[snapBack].img.scale.y, &button.pos,
						button.img.scale.x,
						button.img.scale.y))
					{
						//Different items overlapping
						if (indexTmp != snapBack)
						{
							AEVec2Set(&inventoryButton[snapBack].pos, (snapBack % 5) * 90.f - 180.f,
								-(snapBack / 5.f) * 90.f + 180.f);

							ButtonGearUI tmp = button;
							button = inventoryButton[snapBack];
							inventoryButton[snapBack] = tmp;

							if (playerInventory.size() <= indexTmp)
							{
								size_t oldsize = playerInventory.size();
								playerInventory.resize((size_t)indexTmp + 1);
								for (size_t x = oldsize; x < playerInventory.size(); x++)
								{
									playerInventory[x].ID = INVALID_ITEM;
								}
							}
							//Swap items inside inventory
							Inventory::SwapInventory(playerInventory[indexTmp], playerInventory[snapBack]);
							AEVec2Set(&inventoryButton[snapBack].pos, (snapBack % 5) * 90.f - 180.f,
								-(snapBack / 5.f) * 90.f + 180.f);

							AEVec2Set(&button.pos, (indexTmp % 5) * 90.f - 180.f,
								-(indexTmp / 5.f) * 90.f + 180.f);

							snapBack = -1;
							break;
						}
					}
					indexTmp++;
				}

				//check with equipped slots
				for (ButtonGearUI& equipment_slot : equipmentDisplay)
				{
					if (snapBack == -1)
						return;

					if (AETestRectToRect(&inventoryButton[snapBack].pos, inventoryButton[snapBack].img.scale.x, inventoryButton[snapBack].img.scale.y, &equipment_slot.pos, equipment_slot.img.scale.x, equipment_slot.img.scale.y))
					{
						//check if gear location matches with equipment slot location
						if(inventoryButton[snapBack].Item.gear_loc == equipment_slot.Item.gear_loc)
						{
							UseItem(snapBack, inventoryButton[snapBack], *playerReference);
						}


						//Update images
						UpdateInventoryUI();

						audioManager->PlayAudio(false, Audio_List::ITEM_EQUIP);
					}
				}

				//Return to original position
				if (snapBack >= 0)
				{
					AEVec2Set(&inventoryButton[snapBack].pos, (snapBack % 5) * 90.f - 180.f,
						-(snapBack / 5.f) * 90.f + 180.f);
					snapBack = -1;

					audioManager->PlayAudio(false, Audio_List::ITEM_RELEASE);
				}
			}
		}
		

		if (AEInputCheckTriggered(AEVK_RBUTTON))
		{
			UpdateInventoryUI();

			UpdateEquipmentUI();
		}
		//update item position
		UpdateInventory(playerInventory, inventoryButton);
	}

	void DrawInventory(AEGfxVertexList* pWhiteSquareMesh)
	{

		//Inventory images
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		if (Inventory::inventoryOpen)
		{
			f32 x, y;
			AEGfxGetCamPosition(&x, &y);

			AEGfxTextureSet(inventoryBackground.img.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(x, y, 0.f,
				inventoryBackground.img.scale.x,
				inventoryBackground.img.scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


			AEGfxTextureSet(equipmentBackground.img.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(
				equipmentBackground.pos.x + x,
				equipmentBackground.pos.y + y, 0.f,
				equipmentBackground.img.scale.x,
				equipmentBackground.img.scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


			for (ButtonGearUI button : Inventory::equipmentDisplay)
			{
				AEGfxTextureSet(button.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
					button.pos.y + y, 0.f,
					button.img.scale.x, button.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}


			//Player equipment panel details
			f32 width, height;
			std::string playerStatsHeader = "Player Stats: ";
			auto pStats = playerStatsHeader.c_str();
			AEGfxGetPrintSize(fontID, pStats, 0.5f, &width, &height);
			AEGfxPrint(fontID, pStats, -0.75f,
				0.45f - height * 0.5f,
				0.35f, 1, 1, 1, 1);


			auto playerHealth = "Health: " + std::to_string((int)PlayerMaxBasehealth) + " + (" + std::to_string(
				(int)playerReference->GetMaxHealth()) + ")";
			const char* pHealthText = playerHealth.c_str();
			AEGfxGetPrintSize(fontID, pHealthText, 0.5f, &width, &height);
			AEGfxPrint(fontID, pHealthText, -0.75f,
				0.35f - height * 0.5f,
				0.35f, 0, 1, 0, 1);

			auto playerAttack = "Attack: " + std::to_string((int)playerReference->GetWeaponSet().damage);
			const char* pAttackText = playerAttack.c_str();
			AEGfxGetPrintSize(fontID, pAttackText, 0.5f, &width, &height);
			AEGfxPrint(fontID, pAttackText, -0.75f,
				0.25f - height * 0.5f,
				0.35f, 1, 0, 0, 1);


			//ItemInfoDisplay
			if (Inventory::itemHover)
			{
				AEGfxTextureSet(itemDisplayBackground.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(
					itemDisplayBackground.pos.x + x,
					itemDisplayBackground.pos.y + y, 0.f,
					itemDisplayBackground.img.scale.x,
					itemDisplayBackground.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

				auto pText = Inventory::displayItem.Item.name.c_str();
				AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
				AEGfxPrint(fontID, pText, 0.35f,
					0.45f - height * 0.5f,
					0.4f, 1, 1, 1, 1);

				auto quantityString = "Qty: " + std::to_string(Inventory::displayItem.Item.quantity);
				const char* pText1 = quantityString.c_str();
				AEGfxGetPrintSize(fontID, pText1, 0.5f, &width, &height);
				AEGfxPrint(fontID, pText1, 0.35f,
					0.35f - height * 0.5f,
					0.35f, 1, 1, 1, 1);

				AEGfxTextureSet(blank, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(480.f + x,
					90.f + y, 0.f,
					displayItem.img.scale.x * 2,
					displayItem.img.scale.y * 2).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


				AEGfxTextureSet(displayItem.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(480.f + x,
					90.f + y, 0.f,
					displayItem.img.scale.x * 2,
					displayItem.img.scale.y * 2).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

				std::string descriptionHeader = "Description: ";
				auto pDescription = descriptionHeader.c_str();
				AEGfxGetPrintSize(fontID, pDescription, 0.5f, &width, &height);
				AEGfxPrint(fontID, pDescription, 0.35f,
					0.f - height * 0.5f,
					0.35f, 1, 1, 1, 1);

				//Read string & cut the string up to implement newline
				std::vector<std::string> chopped_description = ChopDescription(displayItem.Item.description);

				//auto pText2 = displayItem.Item.description.c_str();
				for(size_t i =0; i<chopped_description.size(); ++i)
				{
					auto pText2 = chopped_description[i].c_str();

					AEGfxGetPrintSize(fontID, pText2, 0.5f, &width, &height);
					AEGfxPrint(fontID, pText2, 0.35f,
						(- 0.05f * i) - height * 0.5f - 0.07f,
						0.25f, 1, 1, 1, 1);
				}
				



				if (displayItem.Item.item_type != MATERIAL)
				{
					if ((displayItem.Item.item_type == FOOD) || (displayItem.Item.item_type ==
						POTION))
					{
						auto healthString = "Healing Amount: " + std::to_string(displayItem.Item.health);
						const char* pText3 = healthString.c_str();
						AEGfxGetPrintSize(fontID, pText3, 0.5f, &width, &height);
						AEGfxPrint(fontID, pText3, 0.35f,
							-0.25f - height * 0.5f,
							0.35f, 1, 1, 1, 1);

						auto attackString = "Atk Buff: " + std::to_string(displayItem.Item.attack);
						const char* pText4 = attackString.c_str();
						AEGfxGetPrintSize(fontID, pText4, 0.5f, &width, &height);
						AEGfxPrint(fontID, pText4, 0.35f,
							-0.35f - height * 0.5f,
							0.35f, 1, 1, 1, 1);
					}
					else
					{
						auto healthString = "Bonus HP: " + std::to_string(displayItem.Item.health);
						const char* pText3 = healthString.c_str();
						AEGfxGetPrintSize(fontID, pText3, 0.5f, &width, &height);
						AEGfxPrint(fontID, pText3, 0.35f,
							-0.25f - height * 0.5f,
							0.35f, 1, 1, 1, 1);

						auto attackString = "Bonus Atk: " + std::to_string(displayItem.Item.attack);
						const char* pText4 = attackString.c_str();
						AEGfxGetPrintSize(fontID, pText4, 0.5f, &width, &height);
						AEGfxPrint(fontID, pText4, 0.35f,
							-0.35f - height * 0.5f,
							0.35f, 1, 1, 1, 1);
					}
				}
			}



			//Inventory list
			for (ButtonGearUI inventory_backing : item_background)
			{
				AEGfxTextureSet(blank, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(inventory_backing.pos.x + x,
					inventory_backing.pos.y + y, 0.f,
					inventory_backing.img.scale.x,
					inventory_backing.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}

			for (ButtonGearUI button : inventoryButton)
			{
				if (button.Item.ID != INVALID_ITEM)
				{
					AEGfxTextureSet(button.img.pTex, 0, 0);
					AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
						button.pos.y + y, 0.f,
						button.img.scale.x, button.img.scale.y).m);
					AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
				}
			}


		}

	}


	void AddItem(const Item& item)
	{
		
		if(item.stackable)
		{
			for (auto& playeritem : playerInventory)
			{
				//check if got existing ID, item++
				if ((playeritem.ID == item.ID))
				{
					playeritem.quantity++;
					return;
				}
			}
		}

		//Check for empty spaces in inventory
		for (auto& playeritem : playerInventory)
		{
			//Add new item to empty space if found
			if(playeritem.ID < 0)
			{
				playeritem = item;
				return;
			}
		}
		//if no empty spaces found, add to the back of list
		playerInventory.push_back(item);

		UpdateInventory(playerInventory, inventoryButton);
	}
	
	//Function to apply the effect of a consumable item on the player
	void ApplyConsumableEffect(class Player& player, const Item& item)
	{
			// Apply the effect of the item on the player
			 player.RecoverHpToPlayer(item.health);

			 

	}

	void UseItem(int index, ButtonGearUI& item, class Player& player)
	{
		//ButtonGearUI has properties like img, isWeapon, etc.
		//equipmentDisplay is an array or vector representing equipped items

		// Check if the item is available in the player's inventory
		if (index >= 0 && index < MAX_INVENTORY_SIZE)
		{
				// Perform actions based on the type of item equipped
				if (item.Item.item_type == FOOD || item.Item.item_type == POTION)
				{
					// For non-weapon items
					// Adjust player attributes or perform other actions
					// Example: Increase player's health if certain conditions are met
					
					if (playerInventory[index].quantity > 0)
					{
						
						std::cout << "Consumed " << item.Item.name << std::endl;

						// Apply item effect
						ApplyConsumableEffect(player, item.Item);
			
						// Example: Reduce the quantity of the consumed item
						playerInventory[index].quantity -= 1;
					
						std::cout << "Quantity of " << playerInventory[index].name << " reduced to " << playerInventory[index].quantity << std::endl;

						
					}

				}
				else if(item.Item.item_type == WEAPON || item.Item.item_type == ARMOUR)
				{
						std::cout << "Equipped " << item.Item.name << std::endl;

					 switch (item.Item.item_type)
					 {
					 case WEAPON:
						 switch (item.Item.rarity)
						 {
						 case COMMON:
						 case RARE:
						 case EPIC:
							 Equip_Weapon(player, Weapon_System::WEAPON_GRADE::TIER_1);
							 break;
						 case LEGENDARY:
							 Equip_Weapon(player, Weapon_System::WEAPON_GRADE::TIER_2);
							 break;
						 case UNIQUE:
							 Equip_Weapon(player, Weapon_System::WEAPON_GRADE::TIER_3);
							 break;
						 }
					 	break;
					
					 case ARMOUR:
					
					 	switch (item.Item.gear_loc)
					 	{
					 	case head:
					
					 		switch (item.Item.rarity)
					 		{
					 		case COMMON:
								std::cout << "Common: " << item.Item.name << std::endl;
					 			Equip_Armor(player, Armor_System::ARMOR_TYPE::HEAD, Armor_System::ARMOR_GRADE::TIER_1);
					 			break;
					 		case RARE:
							case LEGENDARY:
								std::cout << "Rare: " << item.Item.name << std::endl;
					 			Equip_Armor(player, Armor_System::ARMOR_TYPE::HEAD, Armor_System::ARMOR_GRADE::TIER_2);
					 			break;
					 		case EPIC:
								std::cout << "Epic: " << item.Item.name << std::endl;
					 			Equip_Armor(player, Armor_System::ARMOR_TYPE::HEAD, Armor_System::ARMOR_GRADE::TIER_3);
					 			break;
					
					 	}
					 	break;
					
					 case body:
					 	switch (item.Item.rarity)
					 	{
					 	case COMMON:
					
					 		Equip_Armor(player,   Armor_System::ARMOR_TYPE::BODY, Armor_System::ARMOR_GRADE::TIER_1);
					
					 		break;
					 	case RARE:
						case LEGENDARY:
					 		Equip_Armor(player,   Armor_System::ARMOR_TYPE::BODY, Armor_System::ARMOR_GRADE::TIER_2);
					 		break;
					 	case EPIC:
					 		Equip_Armor(player,   Armor_System::ARMOR_TYPE::BODY, Armor_System::ARMOR_GRADE::TIER_3);
					 		break;
					 	}
					 	break;
					 case pants:
					 	switch (item.Item.rarity)
					 	{
					 	case COMMON:
					
					 		Equip_Armor(player,  Armor_System::ARMOR_TYPE::LEGS, Armor_System::ARMOR_GRADE::TIER_1);
					
					 		break;
					 	case RARE:
						case LEGENDARY:
					 		Equip_Armor(player,  Armor_System::ARMOR_TYPE::LEGS, Armor_System::ARMOR_GRADE::TIER_2);
					 		break;
					 	case EPIC:
					 		Equip_Armor(player,  Armor_System::ARMOR_TYPE::LEGS, Armor_System::ARMOR_GRADE::TIER_3);
					 		break;
					 	}
					 	break;
					 case boots:
					 	switch (item.Item.rarity)
					 	{
					 	case COMMON:
					
					 		Equip_Armor(player,  Armor_System::ARMOR_TYPE::FOOT, Armor_System::ARMOR_GRADE::TIER_1);
					
					 		break;
					 	case RARE:
						case LEGENDARY:
					 		Equip_Armor(player,  Armor_System::ARMOR_TYPE::FOOT, Armor_System::ARMOR_GRADE::TIER_2);
					 		break;
					 	case EPIC:
					 		Equip_Armor(player,  Armor_System::ARMOR_TYPE::FOOT, Armor_System::ARMOR_GRADE::TIER_3);
					 		break;
					 	}
					 	break;
					 	}
					 	break;
					 }
						
					Item equipping = item.Item;
					item.Item = emptySpace;
					item.img.pTex = blank;
					playerInventory[index].ID = INVALID_ITEM;
					playerInventory[index] = emptySpace;


					EquipItemLogic(equipping);
					
					audioManager->PlayAudio(false, Audio_List::ITEM_EQUIP);
				}
		}
		else
		{
			// Handle invalid index (out of range)
			// Example: Display an error message or log a warning
			std::cerr << "Error: Invalid inventory index\n";
		}
	}

	// Function to update the player's stats after equipping or unequipping items
	void UpdatePlayerGearStats(const std::vector< Item>& equippedItems)
	{
		for (const auto& gear : equippedItems)
		{
			switch (gear.item_type)
			{
			case WEAPON:
				switch (gear.rarity)
				{
				case COMMON:
				case RARE:
				case EPIC:
					Equip_Weapon(*playerReference, Weapon_System::WEAPON_GRADE::TIER_1);
					break;
				case LEGENDARY:
					Equip_Weapon(*playerReference, Weapon_System::WEAPON_GRADE::TIER_2);
					break;
				case UNIQUE:
					Equip_Weapon(*playerReference, Weapon_System::WEAPON_GRADE::TIER_3);
					break;
				case IR_NONE:
					Equip_Weapon(*playerReference, Weapon_System::WEAPON_GRADE::NO_GRADE);
						break;

				}
				break;

			case ARMOUR:

				switch (gear.gear_loc)
				{
				case head:

					switch (gear.rarity)
					{
					case COMMON:
						std::cout << "Common: " << gear.name << std::endl;
						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::HEAD, Armor_System::ARMOR_GRADE::TIER_1);
						break;
					case RARE:
					case LEGENDARY:
						std::cout << "Rare: " << gear.name << std::endl;
						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::HEAD, Armor_System::ARMOR_GRADE::TIER_2);
						break;
					case EPIC:
						std::cout << "Epic: " << gear.name << std::endl;
						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::HEAD, Armor_System::ARMOR_GRADE::TIER_3);
						break;
					case IR_NONE:
						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::HEAD, Armor_System::ARMOR_GRADE::NO_GRADE) ;
						break;
					}
					break;

				case body:
					switch (gear.rarity)
					{
					case COMMON:

						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::BODY, Armor_System::ARMOR_GRADE::TIER_1);

						break;
					case RARE:
					case LEGENDARY:
						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::BODY, Armor_System::ARMOR_GRADE::TIER_2);
						break;
					case EPIC:
						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::BODY, Armor_System::ARMOR_GRADE::TIER_3);
						break;
					case IR_NONE:
						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::BODY, Armor_System::ARMOR_GRADE::NO_GRADE);
						break;

					}
					break;
				case pants:
					switch (gear.rarity)
					{
					case COMMON:

						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::LEGS, Armor_System::ARMOR_GRADE::TIER_1);

						break;
					case RARE:
					case LEGENDARY:
						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::LEGS, Armor_System::ARMOR_GRADE::TIER_2);
						break;
					case EPIC:
						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::LEGS, Armor_System::ARMOR_GRADE::TIER_3);
						break;
					case IR_NONE:
						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::LEGS, Armor_System::ARMOR_GRADE::NO_GRADE);
						break;
					}
					break;
				case boots:
					switch (gear.rarity)
					{
					case COMMON:

						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::FOOT, Armor_System::ARMOR_GRADE::TIER_1);

						break;
					case RARE:
					case LEGENDARY:
						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::FOOT, Armor_System::ARMOR_GRADE::TIER_2);
						break;
					case EPIC:
						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::FOOT, Armor_System::ARMOR_GRADE::TIER_3);
						break;
					case IR_NONE:
						Equip_Armor(*playerReference, Armor_System::ARMOR_TYPE::FOOT, Armor_System::ARMOR_GRADE::NO_GRADE);
						break;
					}
					break;
				}
				break;
			}


		}
	}

	std::vector<std::string> ChopDescription(std::string& string)
	{
		std::vector<std::string> result;
		std::string store="";
		for (auto c : string )
		{
			if(c == '@')
			{
				c = '\n';
				result.push_back(store);
				store = "";
			}
			if (c != '\n')
			{
				store.push_back(c);
			}
		}
		result.push_back(store);
		return result;
	}


	void EquipItemLogic(Item obj)
	{
		
		for(int i = 0; i< 5; ++i)
		backupPreviousItem[i] = emptySpace;


			switch (obj.gear_loc)
			{
			case weaponry:
			if (equippedGear[obj.gear_loc].ID >= 0)
			{
				backupPreviousItem[obj.gear_loc] = equippedGear[obj.gear_loc];
			}

			equippedGear[obj.gear_loc] = obj;
			equipmentDisplay[obj.gear_loc].Item = equippedGear[obj.gear_loc];

			break;


			case head:

				if (equippedGear[obj.gear_loc].ID >= 0)
				{
					backupPreviousItem[obj.gear_loc] = equippedGear[obj.gear_loc];
				}

			equippedGear[obj.gear_loc] = obj;
			equipmentDisplay[obj.gear_loc].Item = equippedGear[obj.gear_loc];
			break;

			case body:
				if (equippedGear[obj.gear_loc].ID >= 0)
				{
					backupPreviousItem[obj.gear_loc] = equippedGear[obj.gear_loc];

				}

			equippedGear[obj.gear_loc] = obj;
			equipmentDisplay[obj.gear_loc].Item = equippedGear[obj.gear_loc];


			break;

			case pants:

				if (equippedGear[obj.gear_loc].ID >= 0)
				{
					backupPreviousItem[obj.gear_loc] = equippedGear[obj.gear_loc];

				}
			equippedGear[obj.gear_loc] = obj;
			equipmentDisplay[obj.gear_loc].Item = equippedGear[obj.gear_loc];


			break;

			case boots:

				if (equippedGear[obj.gear_loc].ID >= 0)
				{
					backupPreviousItem[obj.gear_loc] = equippedGear[obj.gear_loc];

				}
				equippedGear[obj.gear_loc] = obj;
			equipmentDisplay[obj.gear_loc].Item = equippedGear[obj.gear_loc];


			break;
		default:
			break;

			}

		//Assign the previously equipped item to empty inventory slot
		for (auto& inventory : playerInventory)
		{
			if (inventory.ID < 0) //check for invalid item id
			{
				inventory = backupPreviousItem[obj.gear_loc];
				break;
			}
		}
	}

	void UnequipItem(const Gear_Location slot)
	{
		if(playerInventoryCount + 1 > MAX_INVENTORY_SIZE)
		{
			std::cout << "Inventory full!";
			return;
		}
		equippedGear[slot].rarity = IR_NONE;
		UpdatePlayerGearStats(equippedGear);

		equippedGear[slot] = emptySpace;
		equipmentDisplay[slot].Item = emptySpace;


		std::cout << "Unequipped called" << std::endl;
		audioManager->PlayAudio(false, Audio_List::ITEM_UNEQUIP);
		
	}

	void LoadInventory()
	{
		std::string player_filepath = "";

		if (isNewAccount)
		{
			player_filepath = "Assets/SaveFiles/player_inventory_new.json";
			playerReference->GetMaxHealth() = PlayerMaxBasehealth;
			Armor_System::Armor d_ArmorPiece;

			for (size_t index = 0; index < 4; index++)
				playerReference->GetArmorSet().pieces[index] = d_ArmorPiece;
			Weapon_System::Equip_Weapon(*playerReference, Weapon_System::WEAPON_GRADE::NO_GRADE);

			for (auto& gear : equippedGear)
			{
				gear = emptySpace;
			}
		}else if (isGodAccount)
		{
			player_filepath = "Assets/SaveFiles/god_inventory.json";
			equippedGear = ReadJsonFile("Assets/SaveFiles/equipped_god_gears.json");
		}
		else
		{
			player_filepath = "Assets/SaveFiles/player_inventory.json";
			equippedGear = ReadJsonFile("Assets/SaveFiles/equipped_gears.json");
		}
		playerInventory = ReadJsonFile(player_filepath);
		fullInventoryList = ReadJsonFile("Assets/SaveFiles/full_item_list.json");

		inventoryBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");
		equipmentBackground.img.pTex = AEGfxTextureLoad("Assets/UI_Sprite/ItemPanel/display_item_left.png");
		itemDisplayBackground.img.pTex = AEGfxTextureLoad("Assets/UI_Sprite/ItemPanel/display_item_right.png");

		blank = AEGfxTextureLoad("Assets/panelInset_beige.png");

		//Item images
		for (size_t i = 0; i < fullInventoryList.size(); ++i)
		{
			// Construct the file path for the texture
			std::stringstream ss;
			ss << "Assets/items/item_" << i << ".png";
			std::string filePath = ss.str();

			// Load the texture using the constructed file path
			Gear[i] = AEGfxTextureLoad(filePath.c_str());
		}


		//check if item slot is same as gear loc
		//if not same store into tmp
		//current will be empty
		for (size_t i = 0; i < 5; ++i)
		{
			if (equippedGear.size() < 5)
			{
				equippedGear.push_back(emptySpace);
			}
			else if (equippedGear.size() > 5)
			{
				equippedGear.pop_back();
			}
		}



		std::vector<Item> tmp = equippedGear;

		for (size_t i = 0; i < 5; ++i)
		{
			for (size_t j=0; j< 5; ++j)
			{
				if (static_cast<Gear_Location> (i) == equippedGear[j].gear_loc)
				{
					tmp[i] = equippedGear[j];
					break;
				}else
				{
					tmp[i] = emptySpace;
					tmp[i].gear_loc = static_cast<Gear_Location> (i);
				}
			}


			if ((equippedGear[i].ID <0 ))
			{
				//fillup equippedGear vector with empty elements if less than maxslots occupied
				equippedGear[i] = emptySpace;
				equipmentDisplay[i].Item = equippedGear[i];
				//Init the empty slots, this will match to the enum values
				equipmentDisplay[i].Item.gear_loc = (Gear_Location)i;
			}
		}
		equippedGear = tmp;

		//Initialise the UI based on equippedGear list
		for (size_t i = 0; i < 5; ++i)
		{
			equipmentDisplay[i].Item = equippedGear[i];
		}
	}

	void InitInventory()
	{
		UpdateInventory(playerInventory, inventoryButton);

		if(!isNewAccount)
		{
			for (auto& gear : equippedGear)
			{

				//std::cout << "check: " << gear.name << std::endl;
				EquipItemLogic(gear);

			}
		}
		isNewAccount = false;

		UpdatePlayerGearStats(equippedGear);

		AEVec2Set(&inventoryBackground.img.scale, 500.f, 500.f);

		inventoryOpen = false;
		s16 index = 0;

		snapBack = -1;

		//Display inventory
		AEVec2Set(&equipmentBackground.img.scale, 400.f, 500.f);
		AEVec2Set(&equipmentBackground.pos, -450.f, 0.f);

		//Item Info Display
		AEVec2Set(&itemDisplayBackground.img.scale, 400.f, 500.f);
		AEVec2Set(&itemDisplayBackground.pos, 450.f, 0.f);

		index = 0;
		for (ButtonGearUI& button : Inventory::equipmentDisplay)
		{
			if(button.Item.ID<0)
			{
				button.img.pTex = blank;
			}else
			{
				button.img.pTex = Gear[button.Item.ID];
			}
			//Set scale
			AEVec2Set(&button.img.scale, 90.f, 90.f);
			index++;
		}
		//Set custom position
		AEVec2Set(&equipmentDisplay[0].pos, -550.f, 30.f);
		AEVec2Set(&equipmentDisplay[1].pos, -350.f, 30.f);

		AEVec2Set(&equipmentDisplay[2].pos, -450.f, -65.f);

		AEVec2Set(&equipmentDisplay[3].pos, -550.f, -160.f);
		AEVec2Set(&equipmentDisplay[4].pos, -350.f, -160.f);


	}

	void SaveInventory()
	{
		if (isGodAccount)
		{
			WriteJsonFile(playerInventory, "Assets/SaveFiles/god_inventory.json");
			WriteJsonFile(equippedGear, "Assets/SaveFiles/equipped_god_gears.json");
		}
		else
		{
			WriteJsonFile(playerInventory, "Assets/SaveFiles/player_inventory.json");
			WriteJsonFile(equippedGear, "Assets/SaveFiles/equipped_gears.json");
		}
	}

	void FreeInventory()
	{
		AEGfxTextureUnload(blank);
		AEGfxTextureUnload(inventoryBackground.img.pTex);
		AEGfxTextureUnload(equipmentBackground.img.pTex);
		AEGfxTextureUnload(itemDisplayBackground.img.pTex);

		//free item images
		for (size_t i = 0; i < fullInventoryList.size(); ++i)
		{
			AEGfxTextureUnload(Gear[i]);
		}
	}
}