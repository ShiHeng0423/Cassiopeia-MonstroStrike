#pragma once
#include <stdlib.h>
#include <vector>
#include <string>
#include "CollisionShape.h" //Collision checker
#include "GridTypesList.h"

#define MAP_ROW_SIZE 50
#define MAP_COLUMN_SIZE 98

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

	GRID_TYPES typeOfGrid;

	AABB collisionBox; //Rectangle collision box

};

bool MapLoader(const char* csvFilePath, std::vector<std::vector<MapCell>>& map, int rows, int cols);

void PrintMap(const std::vector<std::vector<MapCell>>& map, int rows, int cols);

void InitializeGrid(Grids2D& theGrids);