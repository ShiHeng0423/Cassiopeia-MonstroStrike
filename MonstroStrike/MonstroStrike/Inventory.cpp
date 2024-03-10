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

using namespace rapidjson;

std::vector<Item> playerInventory;
int Player_Inventory_Count;
Player* playerReference = nullptr;
Item equippedArmour[4];
Item equippedWeapon;
AEGfxTexture* Gear[25];

namespace Inventory
{
	//Global Variable
	
	std::vector<Item> allItems; //list of all items in game
	ButtonGearUI equipmentDisplay[5];

	std::vector< Item> ReadJsonFile(const std::string& filepath)
	{
		Item nothing{};
		nothing.ID = -99999;
		equippedWeapon = nothing;
		for (int i =0; i< 4; ++i)
		{
			equippedArmour[i] = nothing;
		}

		
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

			printf("\nAccess items in Inventory:\n");
			assert(json.IsObject());
			// Document is a JSON value represents the root of DOM. Root can be either an object or array.

			assert(json["items"].IsArray());
			const Value& items = json["items"];

			for (SizeType loc = 0; loc < items.Size(); loc++)
			{
				//std::cout << loc << std::endl;

				const Value& ind_item = items[loc];

				Item newItem;

				newItem.UID = ind_item["UID"].GetString();
				newItem.ID = ind_item["ID"].GetInt();
				newItem.name = ind_item["name"].GetString();
				newItem.description = ind_item["description"].GetString();
				newItem.item_type = static_cast<Item_Type>(ind_item["item_type"].GetInt());
				newItem.rarity = static_cast<Rarity> (ind_item["rarity"].GetInt());
				newItem.armour_loc = static_cast<Armour_Location> (ind_item["armour_location"].GetInt());
				newItem.quantity = ind_item["quantity"].GetInt();
				newItem.stackable = ind_item["stackable"].GetBool();
				newItem.health = ind_item["health"].GetInt();
				newItem.attack = ind_item["attack"].GetInt();
				newItem.defence = ind_item["defence"].GetInt();


				// std::cout << newItem.UID << std::endl;
				 std::cout << "ID: " << newItem.ID << std::endl;
				 std::cout << newItem.name << std::endl;
				// std::cout << newItem.description << std::endl;
				// std::cout << newItem.item_type << std::endl;
				// std::cout << "enum" << newItem.rarity << std::endl;
				 std::cout << "Quantity: "<<newItem.quantity << std::endl;
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

		 Document json;
		 json.SetObject();

		Value items( kArrayType);

		// StringBuffer s;
		// Writer<StringBuffer> writer(s);
		//
		// writer.StartObject();


		for (const auto& item : inventory)
		{
			Value ind_item(kObjectType);
			{
				ind_item.AddMember("UID", "hi",json.GetAllocator());
			}
			ind_item.AddMember("UID", Value(item.UID.c_str(), json.GetAllocator()), json.GetAllocator());
			ind_item.AddMember("ID", item.ID, json.GetAllocator());
			ind_item.AddMember("name", Value(item.name.c_str(), json.GetAllocator()), json.GetAllocator());
			ind_item.AddMember("description", Value(item.description.c_str(), json.GetAllocator()),
			                   json.GetAllocator());
			ind_item.AddMember("item_type", item.item_type, json.GetAllocator());
			ind_item.AddMember("rarity", item.rarity, json.GetAllocator());
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

		std::cout << buffer.GetString() << std::endl;

		
		 std::ofstream ofs(filepath);



		if (ofs.is_open())
		{
			ofs << buffer.GetString();
			ofs.close();
			std::cout << "Successfully wrote to the JSON file: " << filepath << std::endl;
		}
		else
		{
			std::cerr << "Failed to open the output JSON file: " << filepath << std::endl;
		}

	}





	//Update inventory vector every frame
	void UpdateInventory(const std::vector<Item>& inventory, ButtonGearUI button[])
	{
		Player_Inventory_Count = 0;
		for(size_t i=0; i<inventory.size(); ++i)
		{
			button[i].Item = inventory[i];
			Player_Inventory_Count++;
			
		}
		if(inventory.size()<25)
		{
			for(size_t i = inventory.size() ; i < 25 ; i++)
			{
				Item emptyItemSlot;
				emptyItemSlot.ID = -999999;
				button[i].Item = emptyItemSlot;
			}
		}
	}


		void SwapInventory(Item& lhs, Item& rhs)
		{
			Item tmp = lhs;
			lhs = rhs;
			rhs = tmp;

		}

		// Function to get an item by its ID
		Item getItemById(int id) {
			// Iterate through all items to find the one with the matching ID
			for (const Item& item : allItems) {
				if (item.ID == id) {
					return item;
				}
			}
			// Return a default item if the ID is not found
			return Item{ "Unknown Item", -1, "Forbidden Fruit", "So black, it can't be seen, ever...", FOOD,UNIQUE,AL_NONE, true, 1,1 };
		}


	void Item_Pickup(Item& item)
	{
	}

	void Item_Drop()
	{
	}

	//Function to apply the effect of a consumable item on the player
	void applyItemEffect(Player& player, const Item& item)
	{
		// Check if the item is a consumable (food or potion)
		if (item.item_type == Item_Type::FOOD || item.item_type == Item_Type::POTION)
		{
			// Apply the effect of the item on the player

			// player.max_health += static_cast<f32> (item.health);
			// player.attack += static_cast<f32> (item.attack);
			// player.defence += static_cast<f32> (item.defence);
			//
			// std::cout << "Increased by " << item.health << " Current hp = "  << player.max_health << std::endl;
			// std::cout << "Attack increased by " << item.attack << " Current atk = " << player.attack << std::endl;
			// std::cout << "Defense increased by " << item.defence << " Current df = " << player.defence << std::endl;

		}
		else
		{
			std::cerr << "Error: Cannot apply effect. Item is not a consumable." << std::endl;
		}
	}


	// Function to update the player's stats after equipping or unequipping items
	void updatePlayerStats(Player& player, const std::vector<Item>& equippedItems)
	{
		// Reset player's stats to base values
		// player.hp = player.maxHp;
		// player.attack = 0;
		// player.defence = 0;

		// Iterate through equipped items and update player's stats accordingly
		for (const auto& item : equippedItems)
		{
			// Update player's attack based on equipped weapon(s)
			if (item.item_type == Item_Type::WEAPON)
			{
				//player.attack += item.attack;
			}
			// Update player's defence based on equipped armor(s)
			else if (item.item_type == Item_Type::ARMOUR)
			{
				//player.health += item.health;
				//player.defence += item.defence;
			}
			// Apply other effects of equipped items as needed
			// For example: increase player's maxHp, mana, agility, etc.
		}
	}


	void UseItem(int index, ButtonGearUI& item, Player& player)
	{
		//ButtonGearUI has properties like img, isWeapon, etc.
		//equipmentDisplay is an array or vector representing equipped items

		const int MAX_INVENTORY_SIZE = 25;
		const int MAX_EQUIPPED_ITEMS = 5;

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
						applyItemEffect(player, item.Item);
			
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
						Item equipping = item.Item;
						Item blank;
						blank.ID = -9999;
						item.Item = blank;
						playerInventory[index].ID = -9999;
						EquipToBody(equipping);
						

					//
					// 	// Remove previous item effect and apply new item effect
					// 	// updatePlayerStats(player);
					// }
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
		switch (obj.item_type)
		{
		case WEAPON:
		{
			
			std::cout << "hi" << std::endl;
				Item backup;
				backup.ID = -9999;
				if (equippedWeapon.ID >= 0)
				{
					backup = equippedWeapon;

				}
				equippedWeapon = obj;
				equipmentDisplay[2].Item = equippedWeapon;
				playerReference->equippedWeapon->damage = (f32)obj.attack;

				for (auto& inventory : playerInventory)
				{
					if (inventory.ID <0 )
					{
						inventory = backup;
						break;
					}
				}
			
			
			
		}
			break;
		case ARMOUR:

			switch (obj.armour_loc)
			{
		case HEAD:
			equippedArmour[0] = obj;
			equipmentDisplay[0].Item = equippedWeapon;
			//playerReference->equippedArmor.defence = obj.defence;
			break;
		case BODY:
			equippedArmour[1] = obj;
			equipmentDisplay[1].Item = equippedWeapon;
			break;
		case PANTS:
			equippedArmour[2] = obj;
			equipmentDisplay[3].Item = equippedWeapon;
			break;
		case BOOTS:
			equippedArmour[3] = obj;
			equipmentDisplay[4].Item = equippedWeapon;
			break;
		default:
			break;

			}

			std::cout << "hi" << std::endl;
			Item backup;
			backup.ID = -9999;
			if (equippedWeapon.ID >= 0)
			{
				backup = equippedWeapon;

			}


			for (auto& inventory : playerInventory)
			{
				if (inventory.ID < 0)
				{
					inventory = backup;
					break;
				}
			}

			break;

		}
	}

	void Load_Inventory()
	{
		playerInventory = ReadJsonFile("Assets/player_inventory.json");

	}

	void InitInventory()
	{
	}

	void SaveInventory()
	{
		WriteJsonFile(playerInventory, "./Assets/saved_player_inventory.json");
	}
}
