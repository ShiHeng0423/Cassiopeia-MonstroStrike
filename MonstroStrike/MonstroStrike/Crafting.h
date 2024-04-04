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
#include <cstdio>
#include <vector>


#include "AEEngine.h"
#include "Inventory.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/istreamwrapper.h"
#pragma once

struct MaterialRequirement
{
	int mat_ID;
	int mat_quantity;
};

struct Recipe
{
	int item_id;
	std::pair<MaterialRequirement, MaterialRequirement> mat_requirements;
};


namespace Crafting
{
	extern std::vector<Recipe> recipeList;

	std::vector<Recipe> ReadRecipes(const std::string& filepath);
	bool Can_Craft(const Recipe recipe, const std::vector<Item> Inventory, int& loc1, int& loc2);
	void Craft_Item(const Recipe recipe, std::vector<Item>& Inventory, const int loc1, const int loc2);


	void LoadRecipes();
}
