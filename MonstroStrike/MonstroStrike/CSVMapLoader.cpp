#include "CSVMapLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

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
                // Accumulate characters until a comma or end of line is encountered
                std::string numberStr;
                while (cellValue != ',' && cellValue != '\n' && lineStream)
                {
                    numberStr += cellValue;
                    lineStream >> cellValue; // Read the next character
                }

                // Convert the accumulated string to an integer
                int number = std::stoi(numberStr);

                MapCell cell;
                cell.symbol = number;
                //std::cout << cell.symbol << ",";
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
        //std::cout << "\n";

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

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
           // std::cout << map[i][j].symbol << ' ';
        }
       // std::cout << std::endl;
    }
}

//This is for printing the map
void InitializeGrid(Grids2D& theGrids)
{
    //Setting up the scale
    theGrids.size.x = GRID_SIZE; // *1.f means cover whole width, *0.1f means 10 tiles per 1600px width map, * 0.01f means 100 tiles
    theGrids.size.y = GRID_SIZE;
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

void SetGridTypes(Grids2D** grids2D, const std::vector<std::vector<MapCell>>& gameMap, int numRows, int numCols) {
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {

            int symbol = gameMap[row][col].symbol;

            switch (symbol) {
            case 0:
                grids2D[row][col].typeOfGrid = EMPTY;
                break;
            case 1:
                grids2D[row][col].typeOfGrid = NORMAL_GROUND;
                break;
            case 2:
                grids2D[row][col].typeOfGrid = HORIZONTAL_PLATFORM_POS;
                break;
            case 3:
                grids2D[row][col].typeOfGrid = VERTICAL_PLATFORM_POS;
                break;
            case 4:
                grids2D[row][col].typeOfGrid = DIAGONAL_PLATFORM_POS;
                break;
            case 5:
                grids2D[row][col].typeOfGrid = LAVA_GRID;
                break;
            case 9:
                grids2D[row][col].typeOfGrid = NONE;
                break;
            case 10:
                grids2D[row][col].typeOfGrid = NPC_BLACKSMITH_A_POS;
                break;
            case 11:
                grids2D[row][col].typeOfGrid = NPC_BLACKSMITH_B_POS;
                break;
            case 12:
                grids2D[row][col].typeOfGrid = NPC_QUEST_GIVER_POS;
                break;
            case 20:
                grids2D[row][col].typeOfGrid = ENEMY_CHARGER_POS;
            break;
            case 21:
                grids2D[row][col].typeOfGrid = ENEMY_FLY_POS;
                break;
            case 22:
                grids2D[row][col].typeOfGrid = ENEMY_SLIME_POS;
                break;
            case 30:
                grids2D[row][col].typeOfGrid = TRAPS_POISON_ARROW_GRID;
                break;
            case 31:
                grids2D[row][col].typeOfGrid = TRAPS_SLOWNESS_ARROW_GRID;
                break;
            case 32:
                grids2D[row][col].typeOfGrid = TRAPS_FIRE_CIRCLE_GRID;
                break;
            case 33:
                grids2D[row][col].typeOfGrid = TRAPS_ROTATING_SAW_GRID;
                break;
            case 91:
                grids2D[row][col].typeOfGrid = PLAYER_POS_GRID_1;
                break;
            case 92:
                grids2D[row][col].typeOfGrid = PLAYER_POS_GRID_2;
                break;
            case 93:
                grids2D[row][col].typeOfGrid = PLAYER_POS_GRID_3;
                break;
            case 94:
                grids2D[row][col].typeOfGrid = PLAYER_POS_GRID_4;
                break;
            case 95:
                grids2D[row][col].typeOfGrid = MAP_TRANSITION_GRID_1;
                break;
            case 96:
                grids2D[row][col].typeOfGrid = MAP_TRANSITION_GRID_2;
                break;
            case 97:
                grids2D[row][col].typeOfGrid = MAP_TRANSITION_GRID_3;
                break;
            case 98:
                grids2D[row][col].typeOfGrid = MAP_TRANSITION_GRID_4;
                break;
            default:
                grids2D[row][col].typeOfGrid = NONE;
                break;
            }
        }
    }
}

void RenderGrids(Grids2D** grids2D, int numRows, int numCols, AEGfxVertexList& mesh)
{
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    for (int rows = 0; rows < numRows; ++rows) {
        for (int cols = 0; cols < numCols; ++cols) {
            switch (grids2D[rows][cols].typeOfGrid)
            {
            case NORMAL_GROUND:
                AEGfxSetColorToMultiply(1.0f, 1.f, 0.0f, 1.0f);
                AEGfxSetTransform(grids2D[rows][cols].transformation.m);
                AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);
                break;
            case EMPTY:
                AEGfxSetColorToMultiply(0.25f, 0.25f, 0.25f, 1.0f);
                AEGfxSetTransform(grids2D[rows][cols].transformation.m);
                AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);
                break;
            case LAVA_GRID:
                AEGfxSetColorToMultiply(0.9f, 0.5f, 0.1f, 1.0f); //Orange
                AEGfxSetTransform(grids2D[rows][cols].transformation.m);
                AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);
                break;
            case MAP_TRANSITION_GRID_1:
            case MAP_TRANSITION_GRID_2:
            case MAP_TRANSITION_GRID_3:
            case MAP_TRANSITION_GRID_4:
                AEGfxSetColorToMultiply(0.0f, 1.f, 0.0f, 1.0f); //Green
                AEGfxSetTransform(grids2D[rows][cols].transformation.m);
                AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);
                break;
            default:
                AEGfxSetColorToMultiply(0.0f, 0.f, 0.0f, 0.0f);
                break;
            }
            AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
}