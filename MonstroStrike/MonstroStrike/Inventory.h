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
#pragma once
#include <crtdbg.h> // To check for memory leaks
#include <string>
#include <fstream>
#include <vector>

#include "AEEngine.h"
#include "Player.h"
#include "Utils.h"
#include "rapidjson/document.h"
// ---------------------------------------------------------------------------


struct Item
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



struct ButtonGearUI
{
	Sprite img;
	AEVec2 pos;
	bool isWeapon;
	Item Item;
};



namespace Inventory
{
	extern std::vector< Item> Player_Inventory;
	extern std::vector<Item> allItems; //list of all items in game
	extern ButtonGearUI equipmentDisplay[5];

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



	//extern std::vector<Item> Player_Inventory;

	std::vector<Item> ReadJsonFile(const std::string& filepath);
	void WriteJsonFile(const std::vector<Item> inventory, const std::string& filepath);
	void InitInventory();
	void Load_Inventory();
	void UpdateInventory(const std::vector<Item>& inventory, ButtonGearUI button[]);
	
	Item getItemById(int id);

	void Item_Pickup(Item& item);
	void Item_Drop();
	void Equip(int index, ButtonGearUI item, Player& player);


	void SaveInventory();
}
