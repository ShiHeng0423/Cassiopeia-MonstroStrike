#include "CSVMapLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

//Author: Johny Yong
//Email: j.yong\@digipen.edu

/*
\brief The function that reads the map and return if the map is successfully loaded into a vector

    Done by opening the file, check if the file is opened, then enter a for loop of row such that i am able to getline per row,
    after getting line is a success, create a stringstream to contain the line and begin to insert the value of each column. The values
    in accordance to row and column is then stored into a vector map that has been passed on as parameter.
 
\param[in] csvFilePath
Name of the file

\param[in, out] map
Reference of the map called in Main.cpp

\param[in] rows
Number of rows

\param[in] cols
Number of cols

\return Result of successful map load
*/
bool MapLoader(const char* csvFilePath, std::vector<std::vector<MapCell>>& map, int rows, int cols) {

    std::ifstream file(csvFilePath, std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << csvFilePath << std::endl;
        return false;
    }

    for (int row = 0; row < rows; ++row) {
        std::string line;
        if (!std::getline(file, line)) {
            std::cerr << "Error reading row " << row + 1 << " from file: " << csvFilePath << std::endl;
            return false;
        }

        std::istringstream lineStream(line);
        std::vector<MapCell> rowCells; // Temporary vector for the current row

        int col = 0;
        char cellValue;
        while (lineStream >> cellValue && col < cols)
        {
            if (cellValue != ',')
            {
                MapCell cell;
                cell.symbol = cellValue - '0';
                rowCells.push_back(cell); // Push the current cell into the row vector
                col++;
            }
        }

        // Ensure that the row vector has enough cells (if cols > number of cells in the row)
        while (col < cols)
        {
            MapCell cell;
            cell.symbol = 0; // Default value for missing cells
            rowCells.push_back(cell);
            col++;
        }

        map.push_back(rowCells); // Push the row vector into the map
    }

    file.close();
    return true;
}

/*
\brief A checker to print the symbol of the map in the file
*/

void PrintMap(const std::vector<std::vector<MapCell>>& map, int rows, int cols)
{
    //std::cout << "In Printer: " << std::endl;

    //for (int i = 0; i < rows; i++)
    //{
    //    for (int j = 0; j < cols; j++)
    //    {
    //        std::cout << map[i][j].symbol << ' ';
    //    }
    //    std::cout << std::endl;
    //}
}

//This is for printing the map
void InitializeGrid(Grids2D& theGrids)
{
    //Setting up the scale
    theGrids.size.x = AEGfxGetWindowWidth() * 0.025f; // *1.f means cover whole width, *0.1f means 10 tiles per 1600px width map, * 0.01f means 100 tiles
    theGrids.size.y = AEGfxGetWindowWidth() * 0.025f;
    theGrids.scale = { 0 };
    AEMtx33Scale(&theGrids.scale, theGrids.size.x, theGrids.size.y);

    theGrids.rotation = { 0 };
    AEMtx33Rot(&theGrids.rotation, 0);

    //Positioning the grid
    theGrids.position.x = -AEGfxGetWindowWidth() * 0.5f + theGrids.size.x * (theGrids.colIndex - 0.5f);
    theGrids.position.y = AEGfxGetWindowHeight() * 0.5f - theGrids.size.x * (theGrids.rowIndex - 0.5f);

    AEMtx33Trans(&theGrids.translation, theGrids.position.x, theGrids.position.y);

    theGrids.transformation = { 0 };
    AEMtx33Concat(&theGrids.transformation, &theGrids.rotation, &theGrids.scale);
    AEMtx33Concat(&theGrids.transformation, &theGrids.translation, &theGrids.transformation);

    // Initialize AABB based on the grid's position and size
    theGrids.collisionBox.minimum.x = theGrids.position.x - theGrids.size.x * 0.5f;
    theGrids.collisionBox.minimum.y = theGrids.position.y - theGrids.size.y * 0.5f;
    theGrids.collisionBox.maximum.x = theGrids.position.x + theGrids.size.x * 0.5f;
    theGrids.collisionBox.maximum.y = theGrids.position.y + theGrids.size.y * 0.5f;

    //Implementing physics test
    theGrids.mass = 2.0f;
}

