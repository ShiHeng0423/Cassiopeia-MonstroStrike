///*!*****************************************************************************
//\file    Inventory.h
//\author
//\par
//\par     Course:  csd1451s24
//\par     Section: A
//\par     Software Engineering Project 2
//\date    1-27-2024
//
//\brief
//*******************************************************************************/
//
//// ---------------------------------------------------------------------------
//// includes
//#include "Inventory.h"
//
//
//using json = nlohmann::json;
//
//namespace Inventory
//{
//	std::vector<Inventory> ReadJsonFile(const std::string& filepath)
//	{
//		std::vector<Inventory> inventory;
//
//		std::ifstream ifs(filepath);
//		if (!ifs.is_open())
//		{
//			std::cerr << "Failed to open the JSON file" << std::endl;
//			return inventory;
//		}
//		try
//		{
//			json jsonData;
//			ifs >> jsonData;
//
//			for (const auto& itemData : jsonData["items"])
//			{
//				Inventory newItem;
//				newItem.name = itemData["name"].get<std::string>();
//				newItem.UID = itemData["uid"].get<int>();
//
//				std::cout << newItem.name << std::endl;
//				std::cout << newItem.UID << std::endl;
//			}
//		}
//		catch (const json::exception& e)
//		{
//			std::cerr << "Error parsing JSON: " << e.what() << std::endl;
//		}
//
//		return inventory;
//	}
//
//	void Load_Inventory()
//	{
//		//read JSON file and store into inventory struct
//	}
//
//	void InitInventory()
//	{
//	}
//}
