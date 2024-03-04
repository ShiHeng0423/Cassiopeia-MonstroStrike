/*!*****************************************************************************
\file    Crafting.h
\author
\par
\par     Course:  csd1451s24
\par     Section: A
\par     Software Engineering Project 2
\date    2-27-2024

\brief
*******************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include <crtdbg.h> // To check for memory leaks
#include <string>
#include <fstream>
#include <vector>
#pragma once

#include "AEEngine.h"
#include "Inventory.h"
#include "Utils.h"
#include "rapidjson/document.h"

struct Recipe
{
	int id;
	std::string name;
	std::vector<Item> requirement;
	std::vector<int> qty;
	Item product;
};


namespace Crafting
{
	std::vector<Recipe> ReadRecipes(const std::string filepath);
	bool can_craft(const Recipe recipe, const std::vector<Item> Inventory);
	void craft_item(std::vector<Item>& Inventory);
}
