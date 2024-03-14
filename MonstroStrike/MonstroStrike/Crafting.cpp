#include "Crafting.h"

#include <iostream>
#include <sstream>

#include "rapidjson/error/en.h"


namespace Crafting
{
	std::vector<Recipe> recipeList;


	std::vector<Recipe> ReadRecipes(const std::string& filepath)
	{
		std::vector<Recipe> recipes;

		// Open the JSON file
		std::ifstream ifs(filepath);
		if (!ifs.is_open())
		{
			std::cerr << "Failed to open file: " << filepath << std::endl;
			return recipes;
		}

		rapidjson::Document json;
		rapidjson::IStreamWrapper isw(ifs);
		json.ParseStream(isw);

		// Check if the document is valid 
		if (json.HasParseError() || !json.IsObject() || !json.HasMember("recipes") || !json["recipes"].IsArray())
		{
			std::cerr << "Error: Invalid JSON document OR missing 'recipes' array" << std::endl;
			return recipes;
		}

		const rapidjson::Value& items = json["recipes"];

		// Iterate over the array of recipes
		for (rapidjson::SizeType i = 0; i < items.Size(); ++i)
		{
			const rapidjson::Value& recipeValue = items[i];

			// Extract item_id
			if (!recipeValue.HasMember("item_id") || !recipeValue["item_id"].IsInt())
			{
				std::cerr << "Error: Missing or invalid 'item_id' for recipe at index " << i << std::endl;
				continue; // Skip this recipe and proceed to the next one
			}
			int itemId = recipeValue["item_id"].GetInt();

			// Extract material requirements
			if (!recipeValue.HasMember("material_requirements") || !recipeValue["material_requirements"].IsArray())
			{
				std::cerr << "Error: Missing or invalid 'material_requirements' array for recipe at index " << i <<
					std::endl;
				continue; // Skip this recipe and proceed to the next one
			}
			const rapidjson::Value& matRequirementsValue = recipeValue["material_requirements"];
			if (matRequirementsValue.Size() < 2)
			{
				std::cerr << "Error: Recipe at index " << i << " has less than 2 material requirements" << std::endl;
				continue; // Skip this recipe and proceed to the next one
			}
			MaterialRequirement matReq1, matReq2;
			matReq1.mat_ID = matRequirementsValue[0]["mat_ID"].GetInt();
			matReq1.mat_quantity = matRequirementsValue[0]["mat_quantity"].GetInt();
			matReq2.mat_ID = matRequirementsValue[1]["mat_ID"].GetInt();
			matReq2.mat_quantity = matRequirementsValue[1]["mat_quantity"].GetInt();

			// Create and store the recipe
			recipes.push_back({itemId, {matReq1, matReq2}});
		}

		return recipes;
	}


	void LoadRecipes()
	{
		recipeList = ReadRecipes("Assets/SaveFiles/recipe_list.json");
	}
}
