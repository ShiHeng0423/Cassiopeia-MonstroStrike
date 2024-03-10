#include "Crafting.h"

#include <iostream>
#include <sstream>


std::vector<Recipe> ReadRecipes(const std::string& fileName)
{
	std::vector<Recipe> recipes;
	std::ifstream file(fileName);
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			std::istringstream iss(line);
			Recipe recipe;
			if (!(iss >> recipe.id >> recipe.name))
			{
				std::cerr << "Error: Invalid recipe data\n";
				continue;
			}
			int itemId, itemQty;
			while (iss >> itemId >> itemQty)
			{
				//to get an item from all item list
				Item item = Inventory::getItemById(itemId);
				recipe.requirement.push_back(item);
				recipe.qty.push_back(itemQty);
			}
			// Assuming you have a function to get an item from its ID
			recipe.product = Inventory::getItemById(recipe.id);
			recipes.push_back(recipe);
		}
		file.close();
	}
	else
	{
		std::cerr << "Error: Unable to open file " << fileName << std::endl;
	}
	return recipes;
}
