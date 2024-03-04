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
#include "rapidjson/writer.h"

using namespace rapidjson;



namespace Inventory
{
	//Global Variable
	std::vector< Item> Player_Inventory;
	std::vector<Item> allItems; //list of all items in game
	ButtonGearUI equipmentDisplay[5];

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

			printf("\nAccess items in Inventory:\n");
			assert(json.IsObject());
			// Document is a JSON value represents the root of DOM. Root can be either an object or array.

			assert(json["items"].IsArray());
			const Value& items = json["items"];

			for (SizeType loc = 0; loc < items.Size(); loc++)
			{
				std::cout << loc << std::endl;

				const Value& ind_item = items[loc];

				Item newItem;

				newItem.UID = ind_item["UID"].GetString();
				newItem.ID = ind_item["ID"].GetInt();
				newItem.name = ind_item["name"].GetString();
				newItem.description = ind_item["description"].GetString();
				newItem.item_type = ind_item["item_type"].GetInt();
				newItem.rarity = ind_item["rarity"].GetInt();
				newItem.quantity = ind_item["quantity"].GetInt();
				newItem.stackable = ind_item["stackable"].GetBool();
				newItem.attack = ind_item["attack"].GetInt();
				newItem.defence = ind_item["defence"].GetInt();


				// std::cout << newItem.UID << std::endl;
				 std::cout << newItem.ID << std::endl;
				// std::cout << newItem.name << std::endl;
				// std::cout << newItem.description << std::endl;
				// std::cout << newItem.item_type << std::endl;
				// std::cout << "enum" << newItem.rarity << std::endl;
				// std::cout << newItem.quantity << std::endl;
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


	void WriteJsonFile(const std::vector<Item> inventory, const std::string& filepath)
	{
		std::cout << "start writing JSON" << std::endl;

		 Document json;
		 json.SetObject();

		Value items(kArrayType);

		// StringBuffer s;
		// Writer<StringBuffer> writer(s);
		//
		// writer.StartObject();


		for (const auto& item : inventory)
		{
			Value ind_item(kObjectType);
			ind_item.AddMember("UID", Value(item.UID.c_str(), json.GetAllocator()), json.GetAllocator());
			ind_item.AddMember("ID", item.ID, json.GetAllocator());
			ind_item.AddMember("name", Value(item.name.c_str(), json.GetAllocator()), json.GetAllocator());
			ind_item.AddMember("description", Value(item.description.c_str(), json.GetAllocator()),
			                   json.GetAllocator());
			ind_item.AddMember("item_type", item.item_type, json.GetAllocator());
			ind_item.AddMember("rarity", item.rarity, json.GetAllocator());
			ind_item.AddMember("quantity", item.quantity, json.GetAllocator());
			ind_item.AddMember("stackable", item.stackable, json.GetAllocator());
			ind_item.AddMember("attack", item.attack, json.GetAllocator());
			ind_item.AddMember("defence", item.defence, json.GetAllocator());

			std::cout << item.UID << std::endl;
			std::cout << item.ID << std::endl;
			std::cout << item.name << std::endl;
			std::cout << item.description << std::endl;
			std::cout << item.item_type << std::endl;
			std::cout << "enum" << item.rarity << std::endl;
			std::cout << item.quantity << std::endl;
			std::cout << item.stackable << std::endl;
			std::cout << item.attack << std::endl;
			std::cout << item.defence << std::endl;

			items.PushBack(ind_item, json.GetAllocator());
		}

		json.AddMember("items", items, json.GetAllocator());

		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		json.Accept(writer);

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



	void Load_Inventory()
	{
		Player_Inventory = ReadJsonFile("Assets/player_inventory.json");
		
	}

	//Update inventory vector every frame
	void UpdateInventory(const std::vector<Item>& inventory, ButtonGearUI button[])
	{
		for(SizeType i=0; i<inventory.size(); ++i)
		{
			button[i].Item = inventory[i];
			
		}
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
		return Item{ "Unknown Item", -1, "Forbidden Fruit", "So black, it can't be seen, ever...", food,unique,1, true, 1,1};
	}

	void Item_Pickup(Item& item)
	{
	}

	void Item_Drop()
	{
	}

	void Equip(int index, ButtonGearUI item, Player& player)
	{
		//ButtonGearUI has properties like img, isWeapon, etc.
		//equipmentDisplay is an array or vector representing equipped items

		// Check if the item is available in the player's inventory
		if (index >= 0 && index < 25)
		{
			// Equip the item if it's available
			equipmentDisplay[index].img.pTex = item.img.pTex;

			// Remove the item from the inventory (assuming it's consumed when equipped)
			// @TODO implement more sophisticated logic here
			if (Player_Inventory[index].quantity > 0)
			{
				Player_Inventory[index].quantity--;

				// Perform actions based on the type of item equipped
				if (item.Item.item_type == food || item.Item.item_type == potion)
				{
					// For non-weapon items
					// Adjust player attributes or perform other actions
					// Example: Increase player's health if certain conditions are met
					std::cout << "Consumed " << item.Item.name << std::endl;

				}
				else
				{
					// For weapon items
					// Perform actions specific to weapon equipment
					// Example: Apply a burning effect to the player
					std::cout << "Weapon\n";
				}
			}
		}
		else
		{
			// Handle invalid index (out of range)
			// Example: Display an error message or log a warning
			std::cerr << "Error: Invalid inventory index\n";
		}
	}

	//temporary code section
	// void Equip(int index, ButtonGearUI tmp)
	// {
	// 	equipmentDisplay[index].img.pTex = tmp.img.pTex;
	// 	if (!tmp.isWeapon)
	// 	{
	// 		gear_equipped++;
	// 		if (gear_equipped == 4)
	// 			hp *= 1.5;
	// 	}
	// 	else
	// 	{
	// 		player->burningEffect = true;
	// 	}
	// }

	void InitInventory()
	{
	}

	void SaveInventory()
	{
		WriteJsonFile(Player_Inventory, "Assets/player_inventory.json");
	}
}
