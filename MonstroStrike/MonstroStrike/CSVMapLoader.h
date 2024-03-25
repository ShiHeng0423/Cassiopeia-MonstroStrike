#pragma once
#include <stdlib.h>
#include <vector>
#include <string>
#include "CollisionShape.h" //Collision checker
#include "GridTypesList.h"

#define GRID_SIZE (f32)AEGfxGetWindowWidth() * 0.025f

enum {
	//Level 1 type A
	MAP_ROW_SIZE = 25,
	MAP_COLUMN_SIZE = 70, 

	//Level 1 type B
	MAP_ROW_SIZE_2 = 60,
	MAP_COLUMN_SIZE_2 = 40,

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

void SetGridTypes(Grids2D** grids2D, const std::vector<std::vector<MapCell>>& gameMap, int numRows, int numCols);

void RenderGrids(Grids2D** grids2D, int numRows, int numCols, AEGfxVertexList& mesh);