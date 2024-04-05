/*!************************************************************************
  \file					CSVMapLoader.h
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (100%)
  \brief				File containing declarations of functions and structures for loading and managing CSV map data.

  This file contains declarations of functions and structures essential for loading, managing, and rendering CSV map data
  within the Monstrostrike game. The CSV map loader is responsible for parsing CSV files containing map data and
  converting them into usable game maps represented as 2D grids based on the type of value the CSV file and assign
  what type of grid it is into the data. Additionally, this header defines structures for
  representing map cells and 2D grids, as well as functions for initializing of positions, rendering, and managing these grids.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
#pragma once
#include <stdlib.h>
#include <vector>
#include <string>
#include "CollisionShape.h" //Collision checker
#include "GridTypesList.h"

#define GRID_SIZE (f32)AEGfxGetWindowWidth() * 0.025f

enum {
	//Level 1 type A
	MAP_ROW_SIZE = 27,
	MAP_COLUMN_SIZE = 70, 

	//Level 1 type B
	MAP_ROW_SIZE_2 = 62,
	MAP_COLUMN_SIZE_2 = 40,

	//Lobby Level
	MAP_ROW_LOBBY_SIZE = 27,
	MAP_COLUMN_LOBBY_SIZE = 65, //Shares the same column amount

	//Boss level
	MAP_ROW_BOSS_SIZE = 34,
	MAP_COLUMN_BOSS_SIZE = 52,
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

void GridTextureLoad();

void GridTextureUnload();

bool MapLoader(const char* csvFilePath, std::vector<std::vector<MapCell>>& map, int rows, int cols);

void PrintMap(const std::vector<std::vector<MapCell>>& map, int rows, int cols); //For debugging use

void InitializeGrid(Grids2D& theGrids);

void SetGridTypes(Grids2D** grids2D, const std::vector<std::vector<MapCell>>& gameMap, int numRows, int numCols);

void RenderGrids(Grids2D** grids2D, int numRows, int numCols, AEGfxVertexList& mesh);