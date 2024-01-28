#pragma once
#include <stdlib.h>
#include <vector>
#include <string>

#define MAP_ROW_SIZE 25
#define MAP_COLUMN_SIZE 50

//Reminder to add name space

struct MapCell
{
    char symbol;
};

bool MapLoader(const char* csvFilePath, std::vector<std::vector<MapCell>>& map, int rows, int cols);

void PrintMap(const std::vector<std::vector<MapCell>>& map, int rows, int cols);