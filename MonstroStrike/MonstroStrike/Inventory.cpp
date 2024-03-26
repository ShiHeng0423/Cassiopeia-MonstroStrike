/*!*****************************************************************************
\file    Inventory.h
\author
\par
\par     Course:  csd1451s24
\par     Section: A
\par     Software Engineering Project 2
\date    1-27-2024

\brief
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
#include "main.h"

using namespace rapidjson;

 std::vector< Item> playerInventory;
 std::vector< Item> fullInventoryList;


 AEGfxTexture* Gear[25];
 s16 snapBack = -1;

 Player* playerReference;
 std::vector< Item> equippedGear;

 ButtonGearUI inventoryBackground;
 ButtonGearUI inventoryButton[25];

 ButtonGearUI equipmentBackground;
 ButtonGearUI itemDisplayBackground;

 AEGfxTexture* blank;
 

 int playerInventoryCount;



namespace Inventory
{
	//Global Variable
	
	std::vector<Item> allItems; //list of all items in game
	//std::vector<ButtonGearUI> equipmentDisplay[5];
	ButtonGearUI equipmentDisplay[5];
	bool inventoryOpen;
	bool itemHover;
	ButtonGearUI displayItem;

	std::vector< Item> ReadJsonFile(const std::string& filepath)
	{
		
		//std::vector<Inventory> inventory = new std::vector<Inventory>[1000];
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
			// Document is a JSON value represents the root of DOM. Root can be either an object or array.

			assert(json["items"].IsArray());
			const Value& items = json["items"];

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


				// std::cout << newItem.UID << std::endl;
				// std::cout << "ID: " << newItem.ID << std::endl;
				// std::cout << newItem.name << std::endl;
				// std::cout << newItem.description << std::endl;
				// std::cout << newItem.item_type << std::endl;
				// std::cout << "enum" << newItem.rarity << std::endl;
				//std::cout << "Quantity: "<<newItem.quantity << std::endl;
				// std::cout << newItem.stackable << std::endl;
				// std::cout << newItem.attack << std::endl;
				// std::cout << newItem.defence << std::endl;


				inventory.push_back(newItem);
			}


			if (json.HasParseError())
			{
				std::cerr << "Error parsing JSON: " << std::endl;
			}
		}
		return inventory;
	}

	


	// void Button_Update()
	// {
	// 	for (const auto& obj : Object::ui_queue) {
	// 		switch (obj->object_render_type) {
	// 		case ot_img_button:
	// 			if (point_to_aabb(obj, &Object::mouse))
	// 			{
	// 				
	// 				if (!obj->in_area)
	// 				{
	// 					obj->in_area = true;
	// 					obj->temp_scale = 1.1f;
	//					panel.text = in
	// 				}
	// 			}
	// 			else
	// 			{
	// 				if (obj->in_area)
	// 				{
	// 					obj->in_area = false;
	// 					obj->temp_scale = 1.0;
	//					Hover exit
	//					panel.text = "";
	// 				}
	// 			}
	// 			break;
	// 		default:;
	// 		}
	// 	}
	// }




	void WriteJsonFile(const std::vector<Item>& inventory, const std::string& filepath)
	{
		std::cout << "start writing JSON" << std::endl;

		 Document json = nullptr;
		 json.SetObject();

		Value items( kArrayType);


		for (const auto& item : inventory)
		{
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

		/*	std::cout << item.UID << std::endl;
			std::cout << item.ID << std::endl;
			std::cout << item.name << std::endl;
			std::cout << item.description << std::endl;
			std::cout << item.item_type << std::endl;
			std::cout << "enum " << item.rarity << std::endl;
			std::cout << item.quantity << std::endl;
			std::cout << item.stackable << std::endl;
			std::cout << item.health << std::endl;
			std::cout << item.attack << std::endl;
			std::cout << item.defence << std::endl;*/
			items.PushBack(ind_item, json.GetAllocator());
			//items.AddMember("items",ind_item, json.GetAllocator());
		}

		json.AddMember("items", items, json.GetAllocator());



		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);

		json.Parse(buffer.GetString());

		json.Accept(writer);

		//std::cout << buffer.GetString() << std::endl;

		
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

		for(size_t i=0; i< inventory.size(); ++i)
		{
			button[i].Item = inventory[i];

			if(inventory[i].ID >= 0)
			count++;
			
		}

		if(inventory.size()<25)
		{
			for(size_t i = inventory.size() ; i < 25 ; i++)
			{
				Item emptyItemSlot;
				emptyItemSlot.ID = INVALID_ITEM;
				button[i].Item = emptyItemSlot;
			}
		}

		playerInventoryCount = count;
	}


	void SwapInventory(Item& lhs, Item& rhs)
	{
		Item tmp = lhs;
		lhs = rhs;
		rhs = tmp;

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
		//update item position
		Inventory::UpdateInventory(playerInventory, inventoryButton);
		s16 index = 0;


		for (ButtonGearUI& button : inventoryButton)
		{
			button.img.pTex = blank;
			AEVec2Set(&button.img.scale, 60.f, 60.f);
			AEVec2Set(&button.pos, (int)(index % 5) * 90.f - 180.f, (int)-(index / 5) * 90.f + 180.f);

			if (button.Item.ID < 0)
			{
				button.img.pTex = blank;
			}
			else
			{
				if (button.Item.quantity <= 0)
				{
					Item blank;
					blank.ID = INVALID_ITEM;
					button.Item = blank;
				}
				else
				{
					button.img.pTex = Gear[button.Item.ID];
				}
			}

			//button.isWeapon = false;
			index++;
		}

		//Hover collision with button && hold left mouse button
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			std::cout << "Inventory Count: "<< playerInventoryCount << std::endl;


			s32 textX = 0;
			s32 textY = 0;

			s16 index = 0;

			AEInputGetCursorPosition(&textX, &textY);
			AEVec2 mousePos;
			mousePos.x = textX - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - textY;

			for (ButtonGearUI& button : inventoryButton)
			{
				if (AETestPointToRect(&mousePos, &button.pos, button.img.scale.x, button.img.scale.y))
				{
					if (button.img.pTex != blank)
					{
						//snap origin of img to mouse pos
						snapBack = index;

						//Display item's info on l_click
						//DisplayItemInfo(button.Item);
						itemHover = true;
						displayItem = button;


						break;
					}
					//button.Ptr();
				}
				//Reset itemHover
				itemHover = false;
				displayItem.img.pTex = blank;
				//{"", -9, "invalid item", "",0, 0,5,0,false,0,0,0 };

				index++;
			}
		}

		if (snapBack >= 0)
		{
			s32 textX = 0;
			s32 textY = 0;

			s16 index = 0;

			AEInputGetCursorPosition(&textX, &textY);
			AEVec2 mousePos;
			mousePos.x = textX - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - textY;

			inventoryButton[snapBack].pos = mousePos;
		}

		if (AEInputCheckReleased(AEVK_LBUTTON))
		{
			s16 index = 0;
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
						if (index != snapBack)
						{
							AEVec2Set(&inventoryButton[snapBack].pos, (snapBack % 5) * 90.f - 180.f,
								-(snapBack / 5.f) * 90.f + 180.f);

							//std::cout << "swap\n";
							ButtonGearUI tmp = button;
							button = inventoryButton[snapBack];
							inventoryButton[snapBack] = tmp;

							if (playerInventory.size() <= index)
							{
								size_t oldsize = playerInventory.size();
								playerInventory.resize(index + 1);
								for (size_t x = oldsize; x < playerInventory.size(); x++)
								{
									playerInventory[x].ID = INVALID_ITEM;
								}
							}
							Inventory::SwapInventory(playerInventory[index], playerInventory[snapBack]);
							AEVec2Set(&inventoryButton[snapBack].pos, (snapBack % 5) * 90.f - 180.f,
								-(snapBack / 5) * 90.f + 180.f);

							AEVec2Set(&button.pos, (index % 5) * 90.f - 180.f,
								-(index / 5.f) * 90.f + 180.f);

							snapBack = -1;
							break;
						}
					}
					index++;
				}

				if (snapBack >= 0)
				{
					AEVec2Set(&inventoryButton[snapBack].pos, (snapBack % 5) * 90.f - 180.f,
						-(snapBack / 5.f) * 90.f + 180.f);
					snapBack = -1;
				}
			}
		}
		

		if (AEInputCheckTriggered(AEVK_RBUTTON))
		{
			s16 index = 0;
			s32 textX = 0;
			s32 textY = 0;


			index = 0;
			AEInputGetCursorPosition(&textX, &textY);
			AEVec2 mousePos;
			mousePos.x = textX - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - textY;

			for (ButtonGearUI& button : inventoryButton)
			{
				if (AETestPointToRect(&mousePos, &button.pos, button.img.scale.x, button.img.scale.y))
				{
					if (button.img.pTex != blank)
					{
						Inventory::UseItem(index, button, *playerReference);
						if (button.Item.quantity == 0 || button.Item.ID < 0)
						{
							button.img.pTex = blank;
						}
						break;
					}
				}

				index++;
			}

			index = 0;
			for (ButtonGearUI& button : Inventory::equipmentDisplay)
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
			AEVec2Set(&equipmentDisplay[0].pos, -550.f, -60.f);
			AEVec2Set(&equipmentDisplay[1].pos, -350.f, -60.f);

			AEVec2Set(&equipmentDisplay[2].pos, -450.f, -100.f);

			AEVec2Set(&equipmentDisplay[3].pos, -550.f, -160.f);
			AEVec2Set(&equipmentDisplay[4].pos, -350.f, -160.f);
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
		}else
		{
			for (auto& playeritem : playerInventory)
			{
				if(playeritem.ID < 0)
				{
					playeritem = item;
					return;
				}
			}
			playerInventory.push_back(item);
		}

		UpdateInventory(playerInventory, inventoryButton);
	}


	void ItemPickup(Item& item)
	{
	}

	void ItemDrop()
	{
	}

	//Function to apply the effect of a consumable item on the player
	void ApplyConsumableEffect(class Player& player, const Item& item)
	{
		// Check if the item is a consumable (food or potion)
		if (item.item_type == Item_Type::FOOD || item.item_type == Item_Type::POTION)
		{
			// Apply the effect of the item on the player

			 player.currHealth += static_cast<f32> (item.health);
			// player.attack += static_cast<f32> (item.attack);
			// player.defence += static_cast<f32> (item.defence);
			//
			// std::cout << "Increased by " << item.health << " Current hp = "  << player.max_health << std::endl;
			// std::cout << "Attack increased by " << item.attack << " Current atk = " << player.attack << std::endl;
			// std::cout << "Defense increased by " << item.defence << " Current df = " << player.defence << std::endl;

			//Cap player hp
			 if(playerReference->currHealth > playerReference->maxHealth)
			 {
				 playerReference->currHealth = playerReference->maxHealth;
			 }

		}
		else
		{
			std::cerr << "Error: Cannot apply effect. Item is not a consumable." << std::endl;
		}
	}


	// Function to update the player's stats after equipping or unequipping items
	void ApplyWeaponEffect(Player& player, Item Weapon)
	{
		// Reset player's stats to base values
		// player.hp = player.maxHp;
		// player.attack = 0;
		// player.defence = 0;


			if (Weapon.item_type != WEAPON)
			{
				return;
			}

		
	}


	void UseItem(int index, ButtonGearUI& item, class Player& player)
	{
		//ButtonGearUI has properties like img, isWeapon, etc.
		//equipmentDisplay is an array or vector representing equipped items


		// Check if the item is available in the player's inventory
		if (index >= 0 && index < MAX_INVENTORY_SIZE)
		{
			// @TODO implement more sophisticated logic here

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
					// For weapon items
					// Assign weapon or armour to gear slot
					// if (player.equipment.size() < MAX_EQUIPPED_ITEMS)
					// {
					// 	player.equipment.push_back(item.Item);
						std::cout << "Equipped " << item.Item.name << std::endl;



					 switch (item.Item.item_type)
					 {
					 case WEAPON:
					
					 	break;
					
					 case ARMOUR:
					
					 	switch (item.Item.gear_loc)
					 	{
					 	case head:
					
					 		switch (item.Item.rarity)
					 		{
					 		case COMMON:
					
					 			Equip_Armor(player, player.piece[Armor_System::ARMOR_TYPE::HEAD], Armor_System::ARMOR_TYPE::HEAD, Armor_System::ARMOR_GRADE::TIER_1);
					
					 			break;
					 		case RARE:
					 			Equip_Armor(player, player.piece[Armor_System::ARMOR_TYPE::HEAD], Armor_System::ARMOR_TYPE::HEAD, Armor_System::ARMOR_GRADE::TIER_2);
					 			break;
					 		case EPIC:
					 			Equip_Armor(player, player.piece[Armor_System::ARMOR_TYPE::HEAD], Armor_System::ARMOR_TYPE::HEAD, Armor_System::ARMOR_GRADE::TIER_3);
					 			break;
					
					 	}
					
					
					 	
					
					 	break;
					
					 case body:
					 	switch (item.Item.rarity)
					 	{
					 	case COMMON:
					
					 		Equip_Armor(player, player.piece[Armor_System::ARMOR_TYPE::BODY], Armor_System::ARMOR_TYPE::BODY, Armor_System::ARMOR_GRADE::TIER_1);
					
					 		break;
					 	case RARE:
					 		Equip_Armor(player, player.piece[Armor_System::ARMOR_TYPE::BODY], Armor_System::ARMOR_TYPE::BODY, Armor_System::ARMOR_GRADE::TIER_2);
					 		break;
					 	case EPIC:
					 		Equip_Armor(player, player.piece[Armor_System::ARMOR_TYPE::BODY], Armor_System::ARMOR_TYPE::BODY, Armor_System::ARMOR_GRADE::TIER_3);
					 		break;
					 	}
					 	break;
					 case pants:
					 	switch (item.Item.rarity)
					 	{
					 	case COMMON:
					
					 		Equip_Armor(player, player.piece[Armor_System::ARMOR_TYPE::LEGS], Armor_System::ARMOR_TYPE::LEGS, Armor_System::ARMOR_GRADE::TIER_1);
					
					 		break;
					 	case RARE:
					 		Equip_Armor(player, player.piece[Armor_System::ARMOR_TYPE::LEGS], Armor_System::ARMOR_TYPE::LEGS, Armor_System::ARMOR_GRADE::TIER_2);
					 		break;
					 	case EPIC:
					 		Equip_Armor(player, player.piece[Armor_System::ARMOR_TYPE::LEGS], Armor_System::ARMOR_TYPE::LEGS, Armor_System::ARMOR_GRADE::TIER_3);
					 		break;
					 	}
					 	break;
					 case boots:
					 	switch (item.Item.rarity)
					 	{
					 	case COMMON:
					
					 		Equip_Armor(player, player.piece[Armor_System::ARMOR_TYPE::FOOT], Armor_System::ARMOR_TYPE::FOOT, Armor_System::ARMOR_GRADE::TIER_1);
					
					 		break;
					 	case RARE:
					 		Equip_Armor(player, player.piece[Armor_System::ARMOR_TYPE::FOOT], Armor_System::ARMOR_TYPE::FOOT, Armor_System::ARMOR_GRADE::TIER_2);
					 		break;
					 	case EPIC:
					 		Equip_Armor(player, player.piece[Armor_System::ARMOR_TYPE::FOOT], Armor_System::ARMOR_TYPE::FOOT, Armor_System::ARMOR_GRADE::TIER_3);
					 		break;
					 	}
					 	break;
					
					
					 	}
					
					
					 	break;
					 }
						
					Item equipping = item.Item;
					Item blank;
					blank.ID = INVALID_ITEM;
					item.Item = blank;
					playerInventory[index].ID = INVALID_ITEM;
					EquipToBody(equipping);
					
					// 	Remove previous item effect and apply new item effect
						//ApplyWeaponEffect(player, equippedGear);

				}
			
		}
		else
		{
			// Handle invalid index (out of range)
			// Example: Display an error message or log a warning
			std::cerr << "Error: Invalid inventory index\n";
		}
	}

	void EquipToBody(Item obj)
	{
		Item backup[5];
		for(int i = 0; i< 5; ++i)
		backup[i].ID = INVALID_ITEM;


			switch (obj.gear_loc)
			{
			case weaponry:
			if (equippedGear[obj.gear_loc].ID >= 0)
			{
				backup[obj.gear_loc] = equippedGear[obj.gear_loc];
			}

			equippedGear[obj.gear_loc] = obj;
			equipmentDisplay[obj.gear_loc].Item = equippedGear[obj.gear_loc];

			//update stats
			//playerReference->equippedWeapon.damage = static_cast<f32>(obj.attack);

			break;


			case head:

				if (equippedGear[obj.gear_loc].ID >= 0)
				{
					backup[obj.gear_loc] = equippedGear[obj.gear_loc];
				}

			equippedGear[obj.gear_loc] = obj;
			equipmentDisplay[obj.gear_loc].Item = equippedGear[obj.gear_loc];

			//playerReference->equippedArmor.defence = obj.defence;
			break;

			case body:
				if (equippedGear[obj.gear_loc].ID >= 0)
				{
					backup[obj.gear_loc] = equippedGear[obj.gear_loc];

				}

			equippedGear[obj.gear_loc] = obj;
			equipmentDisplay[obj.gear_loc].Item = equippedGear[obj.gear_loc];


			break;

			case pants:

				if (equippedGear[obj.gear_loc].ID >= 0)
				{
					backup[obj.gear_loc] = equippedGear[obj.gear_loc];

				}
			equippedGear[obj.gear_loc] = obj;
			equipmentDisplay[obj.gear_loc].Item = equippedGear[obj.gear_loc];


			break;

			case boots:

				if (equippedGear[obj.gear_loc].ID >= 0)
				{
					backup[obj.gear_loc] = equippedGear[obj.gear_loc];

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
				
				inventory = backup[obj.gear_loc];
				break;
			}
		}
	}

	void LoadInventory()
	{
		playerInventory = ReadJsonFile("Assets/SaveFiles/player_inventory.json");
		fullInventoryList = ReadJsonFile("Assets/SaveFiles/full_item_list.json");
		equippedGear = ReadJsonFile("Assets/SaveFiles/equipped_gears.json");


		inventoryBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");
		equipmentBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");
		itemDisplayBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");

		blank = AEGfxTextureLoad("Assets/panelInset_beige.png");

		//Item images
		for (size_t i = 0 ; i < fullInventoryList.size(); ++i)
		{
			// Construct the file path for the texture
			std::stringstream ss;
			ss << "Assets/items/item_" << i << ".png";
			std::string filePath = ss.str();

			// Load the texture using the constructed file path
			Gear[i] = AEGfxTextureLoad(filePath.c_str());
		}


		Item emptySpace {"", -999, "", "",
			IT_NONE, IR_NONE, GL_NONE, 0,
			false, 0, 0, 0};

		for (size_t i = 0; i < 5; ++i)
		{
			//fillup equippedGear vector with empty elements if less than maxslots occupied
			if(equippedGear.size() < 5)
			{
				equippedGear.push_back(emptySpace);
			}

			if(equippedGear[i].ID < 0)
			{
				equipmentDisplay[i].Item = emptySpace;
			}else
			{
				equipmentDisplay[i].Item = equippedGear[i];
			}
		}
	}

	void InitInventory()
	{
		UpdateInventory(playerInventory, inventoryButton);

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
		AEVec2Set(&equipmentDisplay[0].pos, -550.f,  -60.f);
		AEVec2Set(&equipmentDisplay[1].pos, -350.f,  -60.f);

		AEVec2Set(&equipmentDisplay[2].pos, -450.f, -100.f);

		AEVec2Set(&equipmentDisplay[3].pos, -550.f, -160.f);
		AEVec2Set(&equipmentDisplay[4].pos, -350.f, -160.f);


	}

	void SaveInventory()
	{
		WriteJsonFile(playerInventory, "Assets/SaveFiles/player_inventory.json");
		WriteJsonFile(equippedGear, "Assets/SaveFiles/equipped_gears.json");
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