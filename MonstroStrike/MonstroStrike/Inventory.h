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
extern int playerInventoryCount;
extern s16 snapBack;
extern Player* playerReference;

//Display
extern ButtonGearUI inventoryBackground;
extern ButtonGearUI inventoryButton[25];
extern ButtonGearUI equipmentBackground;
extern ButtonGearUI itemDisplayBackground;
extern ButtonGearUI item_background[25];

//Textures
extern AEGfxTexture* Gear[25];
extern AEGfxTexture* blank;


#define INVALID_ITEM -999
#define MAX_INVENTORY_SIZE 25

namespace Inventory
{
	extern bool inventoryOpen;
	extern bool itemHover;
	extern std::vector<Item> allItems; //list of all items in game
	extern ButtonGearUI equipmentDisplay[5]; //Array of equipped display

	extern ButtonGearUI displayItem;//Display item information


	std::vector<Item> ReadJsonFile(const std::string& filepath);
	void WriteJsonFile(const std::vector<Item>& inventory, const std::string& filepath);

	void InitInventory();
	void LoadInventory();

	void UpdateInventory(const std::vector<Item>& inventory, ButtonGearUI button[]);
	void UpdateInventoryUI();

	void DrawInventory(AEGfxVertexList* pWhiteSquareMesh);

	void OpenInventory();
	void SwapInventory(Item& lhs, Item& rhs);

	Item GetItemById(int id);

	void AddItem(const Item& item);
	void ItemPickUp(Item& item);
	void ItemDrop();

	void UseItem(int index, ButtonGearUI& item, class Player& player);
	void EquipToBody(Item obj);
	void UnequipItem(Item obj);

	void ApplyConsumableEffect(class Player& player, const Item& item);
	void UpdatePlayerGearStats(const std::vector<Item>& equippedItems);

	std::vector<std::string> ChopDescription(std::string& string);
	//std::string ChopDescription(std::string& string);

	void SaveInventory();
	void FreeInventory();

}