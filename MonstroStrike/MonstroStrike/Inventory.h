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


enum Item_Type
{
	IT_NONE = 0,
	MATERIAL,
	WEAPON,
	ARMOUR,
	FOOD,
	POTION,
	RECIPE
};

enum Rarity
{
	IR_NONE = 0,
	COMMON,
	RARE,
	EPIC,
	LEGENDARY,
	UNIQUE
};

enum Gear_Location
{
	head = 0,
	body,
	weaponry,
	pants,
	boots,
	GL_NONE

};

struct Item
{
	std::string UID;
	int ID;
	std::string name;
	std::string description;

	Item_Type item_type;
	Rarity rarity;
	Gear_Location gear_loc;

	int quantity;
	bool stackable;
	int health;
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

extern std::vector< Item> playerInventory;
extern int Player_Inventory_Count;

extern AEGfxTexture* Gear[25];
extern s16 snapBack;

extern Player* playerReference;
extern std::vector< Item> equippedGear;

extern ButtonGearUI inventoryBackground;
extern ButtonGearUI inventoryButton[25];

extern ButtonGearUI equipmentBackground;

extern AEGfxTexture* blank;


namespace Inventory
{
	
	extern std::vector<Item> allItems; //list of all items in game
	
	//extern std::vector<ButtonGearUI> equipmentDisplay[5];
	extern ButtonGearUI equipmentDisplay[5];

	extern bool inventoryOpen;



	const int inventory_size = 1000;
	const int inventory_height = 5;
	const int inventory_width = 5;



	//extern std::vector<Item> Player_Inventory;

	std::vector<Item> ReadJsonFile(const std::string& filepath);
	void WriteJsonFile(const std::vector<Item>& inventory, const std::string& filepath);
	void InitInventory();
	void LoadInventory();
	void UpdateInventory(const std::vector<Item>& inventory, ButtonGearUI button[]);
	void SwapInventory(Item& lhs, Item& rhs);

	Item GetItemById(int id);

	void OpenInventory();

	void ItemPickUp(Item& item);
	void ItemDrop();


	void EquipToBody(Item obj);
	void ApplyItemEffect(Player& player, const Item& item);
	void UseItem(int index, ButtonGearUI& item, Player& player);
	
	void UpdatePlayerStats(Player& player, const std::vector<Item>& equippedItems);
	void SaveInventory();

	void FreeInventory();

}

void Equip(int index, ButtonGearUI tmp);