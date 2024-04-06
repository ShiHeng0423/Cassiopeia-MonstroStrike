/*!*****************************************************************************
\file				Inventory.h
\project name		Monstrostrike
\primary author		Keith Ng Hon Yew (100%)
\secondary author
\brief				This file contains the declaration of all functions related
					to the inventory system.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
*******************************************************************************/

// ---------------------------------------------------------------------------
// includes
#pragma once
#include <crtdbg.h> // To check for memory leaks
#include <string>
#include <fstream>
#include <vector>

#include "AEEngine.h"
#include "GameStateManager.h"
#include "Player.h"
#include "Utils.h"

#include "rapidjson/document.h"
// ---------------------------------------------------------------------------

enum Account_Slot
{
	ACC_NEW_SLOT = 0,
	ACC_SLOT_1,
};


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

	GL_Total,

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
	int health = 0;
	int attack = 0;
	int defence = 0;
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
extern AEGfxTexture* Gear[29];
extern AEGfxTexture* blank;


#define INVALID_ITEM -999
#define MAX_INVENTORY_SIZE 25

namespace Inventory
{
	extern bool isNewAccount;
	extern bool	isGodAccount;
	extern GameStates fileLoadedState;


	extern bool inventoryOpen;
	extern bool itemHover;
	extern std::vector<Item> allItems; //list of all items in game
	extern ButtonGearUI equipmentDisplay[5]; //Array of equipped display

	extern ButtonGearUI displayItem;//Display item information

	//Item backupPreviousItem[5];


	std::vector<Item> ReadJsonFile(const std::string& filepath);
	void WriteJsonFile(const std::vector<Item>& inventory, const std::string& filepath);

	void InitInventory();
	void LoadInventory();

	void UpdateInventory(const std::vector<Item>& inventory, ButtonGearUI button[]);
	void UpdateInventoryUI();
	void UpdateEquipmentUI();

	void DrawInventory(AEGfxVertexList* pWhiteSquareMesh);

	void OpenInventory();
	void SwapInventory(Item& lhs, Item& rhs);

	Item GetItemById(int id);

	void AddItem(const Item& item);
	void ItemPickUp(Item& item);
	void ItemDrop();

	void UseItem(int index, ButtonGearUI& item, class Player& player);
	void EquipItemLogic(Item obj);
	void UnequipItem(const Gear_Location slot);

	void ApplyConsumableEffect(class Player& player, const Item& item);
	void UpdatePlayerGearStats(const std::vector<Item>& equippedItems);

	std::vector<std::string> ChopDescription(std::string& string);
	//std::string ChopDescription(std::string& string);

	void SaveInventory();
	void FreeInventory();

}