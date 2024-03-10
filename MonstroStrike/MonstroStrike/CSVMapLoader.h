#pragma once
#include <stdlib.h>
#include <vector>
#include <string>
#include "CollisionShape.h" //Collision checker
#include "GridTypesList.h"

enum {
	//Level 1 part A
	MAP_ROW_SIZE = 50,
	MAP_COLUMN_SIZE = 98, 

	//Lobby Level
	MAP_ROW_LOBBY_SIZE = 25,
	MAP_COLUMN_LOBBY_SIZE = 65, //Shares the same column amount

};

//Reminder to add name space

struct MapCell
{
    int symbol;
};

struct Grids2D {

	AEMtx33 scale;
	AEMtx33 rotation;
	AEMtx33 translation;
	AEMtx33 transformation;

	s16 rowIndex;
	s16 colIndex;

	AEVec2 size;
	AEVec2 position;
	AEVec2 velocity;

	f32 mass;

	GridTypes typeOfGrid;

	AABB collisionBox; //Rectangle collision box
};

bool MapLoader(const char* csvFilePath, std::vector<std::vector<MapCell>>& map, int rows, int cols);

void PrintMap(const std::vector<std::vector<MapCell>>& map, int rows, int cols);

void InitializeGrid(Grids2D& theGrids);