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
#include "Inventory.h"

#include <algorithm>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <cstdio>
#include <vector>

#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"

using namespace rapidjson;

namespace Inventory
{
	std::vector<Inventory> ReadJsonFile(const std::string& filepath)
	{
		std::vector<Inventory> inventory;

		std::ifstream ifs(filepath);
		if (!ifs.is_open())
		{
			std::cerr << "Failed to open the JSON file" << std::endl;
			return inventory;
		}
		else 
		{
			Document json;
			IStreamWrapper isw(ifs);
			json.ParseStream(isw);

			// Check if the document is valid 
			if (json.HasParseError()) {
				std::cerr << "Error: failed to parse JSON document"
					<< std::endl;
				ifs.close();
			}

			printf("\nAccess values in document:\n");
			assert(json.IsObject());    // Document is a JSON value represents the root of DOM. Root can be either an object or array.

			//assert(json.HasMember("items"));

			assert(json["items"].IsArray());
			const Value& items = json["items"];

			for(SizeType loc = 0 ; loc < items.Size();loc++)
			{
				std::cout << loc<< std::endl;
				
				const Value& ind_item = items[loc];
			
				//std::cout <<"JSON" <<ind_item["name"].GetString() << std::endl;
				//std::cout << "JSON" << ind_item["UID"].GetString() << std::endl;

				Inventory newItem;

				newItem.UID = ind_item["UID"].GetString();
				newItem.ID = ind_item["ID"].GetInt();
				newItem.name = ind_item["name"].GetString();
				newItem.description = ind_item["description"].GetString();
				newItem.item_type = ind_item["item_type"].GetInt();
				newItem.rarity = ind_item["rarity"].GetInt();
				newItem.quantity = ind_item["quantity"].GetInt();
				newItem.stackable = ind_item["stackable"].GetBool();
				newItem.attack = ind_item["attack"].GetInt();
				newItem.defence = ind_item["defence"].GetInt();


				//std::cout << newItem.UID << std::endl;
				//std::cout << newItem.ID << std::endl;
				//std::cout << newItem.name << std::endl;
				//std::cout << newItem.description << std::endl;
				//std::cout << newItem.item_type << std::endl;
				//std::cout << "enum"<<newItem.rarity << std::endl;
				//std::cout << newItem.quantity << std::endl;
				//std::cout << newItem.stackable << std::endl;
				//std::cout << newItem.attack << std::endl;
				//std::cout << newItem.defence << std::endl;


				inventory.push_back(newItem);

			}


			if (json.HasParseError())
			{
				std::cerr << "Error parsing JSON: " << std::endl;
			}
		}
		return inventory;
	}

	//std::vector<Inventory> SaveToJsonFile(const std::string& filepath,  Inventory& inventory)
	//{
	//	Document json;
	//	assert(json.IsObject());
	//	json.SetObject();

	//	Value out();

	//	//json = inventory;

	//	std::ofstream ofs(filepath);
	//	if (!ofs.is_open())
	//	{
	//		std::cerr << "Failed to open the JSON file" << std::endl;
	//		//return inventory;
	//	}
	////	for (SizeType loc = 0; loc < inventory.Size(); loc++)
	//	{

	//	}

	//	// Writer<Inventory> writer(inventory);
	//	//
	//	// writer.StartObject();
	//	// writer.Key("hello");
	//	// writer.String("world");
	//	// writer.Key("t");
	//	// writer.Bool(true);
	//	// writer.Key("f");
	//	// writer.Bool(false);
	//	// writer.Key("n");
	//	// writer.Null();
	//	// writer.Key("i");
	//	// writer.Uint(123);
	//	// writer.Key("pi");
	//	// writer.Double(3.1416);
	//	// writer.Key("a");
	//	// writer.StartArray();
	//	// for (unsigned i = 0; i < 4; i++)
	//	// 	writer.Uint(i);
	//	// writer.EndArray();
	//	// writer.EndObject();

	//	//std::cout << inventory << std::endl;
	//	
	//	//OStreamWrapper osw(ofs);

	//	// Writer<OStreamWrapper> writer(osw);
	//	// json.Accept(writer);




	//}


	void Load_Inventory()
	{
		ReadJsonFile("Assets/test.json");
	}

	void InitInventory()
	{
	}
}
