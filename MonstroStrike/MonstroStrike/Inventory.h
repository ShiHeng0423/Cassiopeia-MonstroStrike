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
#include <iostream>
#include <fstream>
//#include "json.hpp"
#include <vector>

#include "AEEngine.h"
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


	struct Inventory
	{
		std::string UID;
		int ID;
		std::string name;
		std::string description;
		int item_type;
		int rarity;
		int quantity;
		bool stackable;
		int attack;
		int defence;
	};

	std::vector<Inventory> ReadJsonFile(const std::string& filepath);
	void InitInventory();
	void Load_Inventory();
	void UpdateInventory();

	void Open_Inventory();

	void Item_Pickup();
	void Item_Drop();
	void Item_Equip();
}
