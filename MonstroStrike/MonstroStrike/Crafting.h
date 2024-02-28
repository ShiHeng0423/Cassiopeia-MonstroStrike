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
#include "Utils.h"
#include "rapidjson/document.h"

struct Recipe
{
	int id;
	std::string name;
	std::vector<Inventory::Item> requirement;
	std::vector<int> qty;
	Inventory::Item product;
};


namespace Crafting
{
	std::ifstream ReadRecipes(const std::string filepath);
	bool craftable(const Recipe recipe, const std::vector<Inventory::Item> Inventory);
	void craft_item(std::vector<Inventory::Item>& Inventory);
}
