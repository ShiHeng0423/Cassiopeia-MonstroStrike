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
#include <crtdbg.h> // To check for memory leaks
#include <string>
#include <fstream>
//#include "json.hpp"
#include <vector>

#include "AEEngine.h"
#include "Utils.h"
#include "rapidjson/document.h"
// ---------------------------------------------------------------------------
#pragma once




namespace Inventory
{
	enum Item_Type
	{
		material = 0,
		recipe,
		weapon,
		amour,
		food,
		potion
	};

	enum Rarity
	{
		common = 0,
		rare,
		epic,
		legendary,
		unique
	};

	const int inventory_size = 1000;
	const int inventory_height = 5;
	const int inventory_width = 5;



	extern std::vector<Inventory::Item> Player_Inventory;

	std::vector<Item> ReadJsonFile(const std::string& filepath);
	void WriteJsonFile(const std::vector<Item> inventory, const std::string& filepath);
	void InitInventory();
	void Load_Inventory();
	void UpdateInventory(std::vector<Item>& inventory, ButtonGearUI button[]);
	


	void Item_Pickup(Item& item);
	void Item_Drop();
	void Item_Equip(Item& item);


	void SaveInventory();
}
