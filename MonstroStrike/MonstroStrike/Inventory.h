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

//Inventory list
extern std::vector< Item> playerInventory;
extern std::vector< Item> fullInventoryList;
extern std::vector< Item> equippedGear;

//Misc
extern int Player_Inventory_Count;
extern s16 snapBack;
extern Player* playerReference;




//Display
extern ButtonGearUI inventoryBackground;
extern ButtonGearUI inventoryButton[25];
extern ButtonGearUI equipmentBackground;
extern ButtonGearUI itemDisplayBackground;

//Textures
extern AEGfxTexture* Gear[25];
extern AEGfxTexture* blank;


#define INVALID_ITEM -999;

namespace Inventory
{
	extern bool inventoryOpen;
	extern bool itemHover;
	extern std::vector<Item> allItems; //list of all items in game
	extern ButtonGearUI equipmentDisplay[5]; //Array of equipped display

	//AEGfxVertexList* pWhiteSquareMesh;
	extern Item displayItem;


	std::vector<Item> ReadJsonFile(const std::string& filepath);
	void WriteJsonFile(const std::vector<Item>& inventory, const std::string& filepath);

	void InitInventory();
	void LoadInventory();
	void UpdateInventory(const std::vector<Item>& inventory, ButtonGearUI button[]);


	void SwapInventory(Item& lhs, Item& rhs);
	Item GetItemById(int id);

	void OpenInventory();

	//void DisplayItemInfo(const Item&);

	void AddItem(const Item& item);
	void ItemPickUp(Item& item);
	void ItemDrop();


	void EquipToBody(Item obj);
	void ApplyItemEffect(class Player& player, const Item& item);
	void UseItem(int index, ButtonGearUI& item, class Player& player);
	
	void UpdatePlayerStats(Player& player, const std::vector<Item>& equippedItems);
	void SaveInventory();

	void FreeInventory();

}

